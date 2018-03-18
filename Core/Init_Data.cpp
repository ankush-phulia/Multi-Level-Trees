#include <GL/glew.h>

GLfloat *square_vertex_buffer_data, *square_texture_buffer_data;
int square_vertex_buffer_data_size, square_texture_buffer_data_size;
GLuint squarevertexbuffer, squaretexturebuffer;

void init_Data() {
  square_vertex_buffer_data_size = 2*3*3;
  square_texture_buffer_data_size = 2*3*2;

  square_texture_buffer_data = new GLfloat[square_texture_buffer_data_size];
  square_vertex_buffer_data = new GLfloat[square_vertex_buffer_data_size];

  GLfloat temp_sq_vertex[] = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
  };

  GLfloat temp_sq_texture[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
  };

  for (int i=0; i<square_texture_buffer_data_size; i++) {
    square_texture_buffer_data[i]=temp_sq_texture[i];
  }

  for (int i=0; i<square_vertex_buffer_data_size; i++) {
    square_vertex_buffer_data[i] = temp_sq_vertex[i];
  }

  glGenBuffers(1, &squarevertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, squarevertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, 
    sizeof(GLfloat)*square_vertex_buffer_data_size, 
    square_vertex_buffer_data, GL_STATIC_DRAW);

  glGenBuffers(1, &squaretexturebuffer);
  glBindBuffer(GL_ARRAY_BUFFER, squaretexturebuffer);
  glBufferData(GL_ARRAY_BUFFER, 
    sizeof(GLfloat)*square_vertex_buffer_data_size, 
    square_texture_buffer_data, GL_STATIC_DRAW);
}
