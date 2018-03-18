#include <stdlib.h>
#include <stack>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "./Core/Init_GLFW.cpp"
#include "./Core/Init_Data.cpp"
#include "./Shaders/Shader.cpp"
#include "./Rendering/Imposter.cpp"


GLuint programID, MatrixID; 
GLuint TextureBody, TextureLeg, TextureHead, TextureLegBase;
float ht = 30;
int max_age = 6;
double X, Y, Z;
double theta, theta2;
double shearX, shearZ;

void move() {
    if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
        Z--;
    }
    // Move backward
    if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
    	Z++;
    }
    // Strafe right
    if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
    	X++;
    }
    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
    	X--;
    }
    // Strafe right
    if (glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS){
    	Y++;
    }
    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS){
    	Y--;
    }
    if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
    	// std::cout << theta << std::endl;
    	theta -= 0.01;
    }
    if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
    	// std::cout << theta << std::endl;
    	theta += 0.01;
    }
    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
    	theta2 += 0.01;
    }
    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
    	theta2 -= 0.01;
    }
    if (glfwGetKey( window, GLFW_KEY_BACKSPACE ) == GLFW_PRESS){
    	theta = 0; theta2 = 0;
    	X = 0.0; Y = 20.0; Z = 0.0;
    }
    
}

int main(int argc, char **argv) {
    
    Tree t(GetPositionMatrix(0,0,0),gen_rotation_deg(0, 0, 0));
    t.gen_string("FB", 4);
    int windowWidth = 640;
    int windowHeight = 360;
    // shearX = 0.1, shearZ = 0.1;
    
    srand (static_cast <unsigned> (time(0)));

    // Initialise GLFW
    if(init_GLFW() == -1)
    return -1;

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    programID = LoadShaders( "./Shaders/shader.vs", "./Shaders/shader.fs" );
    programObj = LoadShaders("./Shaders/shader_obj.vs", "./Shaders/shader_obj.fs");
    programTexture = LoadShaders("./Shaders/shader_text.vs","./Shaders/shader_text.fs");
    MatrixID = glGetUniformLocation(programID, "MVP1");
    MatrixIDText = glGetUniformLocation(programTexture, "MVP1");
    ObjMatrixID = glGetUniformLocation(programObj, "MVP");
    ObjViewMatrixID = glGetUniformLocation(programObj, "V");
    ObjModelMatrixID = glGetUniformLocation(programObj, "M");

    Projection = glm::perspective(glm::radians(60.0f), 16.0f / 9.0f, 0.001f, 1000.01f);

    init_Data();

    objfile Leaf_Object("./Models/Leaf.obj",55.0/255, 185.0/255, 55.0/255, 0.1, 0.0);
    objfile Branch_Object("./Models/Branch.obj", 113.0/255, 81.0/255, 81.0/255, 0.3, 0.0);
     
    theta = 0.0; theta2 = 0.0;
    X = 0.0; Y = 20.0; Z = 0.0;

    // The texture we're going to render to
    GLuint *renderedTexture1 = new GLuint[2*Num_Boards*Num_Imposters];

    for(int j=0 ; j<Num_Imposters ; j++){
		Tree *t1 = new Tree(GetPositionMatrix(0,0,0),gen_rotation_deg(0, 0, 0));
        t1->gen_string("FB", rand()%3 + rand()%3);
        GLuint *renderedTexture = &renderedTexture1[j*2*Num_Boards];
        for (int i=0; i<2*Num_Boards; i++) {
            int im = i%Num_Boards;
            Projection = glm::perspective(glm::radians(60.0f), 
                                        16.0f / 9.0f, 
                                        50.0f+im*(60.0f/(0.001f+Num_Boards-1)), 
                                        50.0f+(im+1)*(60.0f/(0.001f+Num_Boards-1)));

            GLuint FramebufferName = 0;
            glGenFramebuffers(1, &FramebufferName);
            glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
            
            glGenTextures(1, &renderedTexture[i]);
            
            glBindTexture(GL_TEXTURE_2D, renderedTexture[i]);

            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, windowWidth, windowHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture[i], 0);
            GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
            // glDrawArrays(GL_TRIANGLES, 0, 3);

            // Always check that our framebuffer is ok
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            return false;

            glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
            glViewport(0,0,windowWidth,windowHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right

            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            View = glm::lookAt(
                        glm::vec3(80*sin(theta),0,80*cos(theta)), // Camera is at (4,3,3), in World Space
                        glm::vec3(0,50,0), // and looks at the origin
                        glm::vec3(0,1,0)    // Head is up (set to 0,-1,0 to look upside-down)
                    );
            t1->render(Leaf_Object,Branch_Object);
            if (i==Num_Boards-1) {
            t1->orientation = gen_rotation_deg(0, 90, 0);
            }

            glfwSwapBuffers(window);
        }
        delete t1;
    }

    double lastTime = glfwGetTime();
    int nbFrames=0;
    Imposters forest[Num_Forests];
    Projection = glm::perspective(glm::radians(60.0f), 16.0f / 9.0f, 0.1f, 10000.0f);
    glDisable(GL_CULL_FACE);
    // glEnable(GL_CULL_FACE);
    do {
        // Measure speed
        // double currentTime = glfwGetTime();
        // nbFrames++;
        // if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
        //     printf("%f ms/frame or %f fps\n", 1000.0/double(nbFrames), double(nbFrames));
        //     nbFrames = 0;
        //     lastTime += 1.0;
        // }
        

        // Render to the screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Render on the whole framebuffer, complete from the lower left corner to the upper right
        glViewport(0,0,2*windowWidth,2*windowHeight);

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        move();
        View = glm::lookAt(
                    glm::vec3(X + 100*sin(theta),Y + ht*sin(theta2),Z + 100*cos(theta)), // Camera is at (4,3,3), in World Space
                    glm::vec3(X,Y,Z), // and looks at the origin
                    glm::vec3(0,1,0)    // Head is up (set to 0,-1,0 to look upside-down)
                 );

        //windy
        // for (int i = 0; i < 2*Num_Boards*Num_Imposters; i++ ) {
        // 	if ((i/Num_Boards)%2 == 0) {

        // 	}
        // }
        float rando = ((float) rand() / (RAND_MAX)) - 0.5;
        rando /= 10;
        shearX += rando;


        rando = ((float) rand() / (RAND_MAX)) - 0.5;
        rando /= 10;
        shearZ += rando;

        for(int i=0; i<Num_Forests ; i++)
        forest[i].render(renderedTexture1, shearX, shearZ);
        
        t.render(Leaf_Object,Branch_Object);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0);

    glDeleteVertexArrays(1, &VertexArrayID);
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    delete [] renderedTexture1;
    return 0;
}
