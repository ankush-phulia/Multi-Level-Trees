#include "Tree.cpp"

#define Num_Boards 5
#define Num_Imposters 10
#define Num_Forests 100

GLuint TextureID, programTexture, MatrixIDText;

void drawSquare(GLuint texture_id, glm::mat4 MVPTexture) {
    glUseProgram(programTexture);

    glUniformMatrix4fv(MatrixIDText, 1, GL_FALSE, &MVPTexture[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUniform1i(TextureID, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, squarevertexbuffer);
    glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but
                             // must match the layout in the shader.
                          3,        // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void*)0  // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, squaretexturebuffer);
    glVertexAttribPointer(1, // attribute 0. No particular reason for 0, but
                             // must match the layout in the shader.
                          2,        // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void*)0  // array buffer offset
    );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0,
                 2 * 3); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void drawImposterTree(GLuint* textures, glm::mat4 modelmat, int level,
                      double shearX, double shearZ) {
    int j = Num_Boards / 2;
    glm::mat4 shearXX = GetShearMatrixX(shearX),
              shearZZ = GetShearMatrixX(shearZ);
    for (int i = 0; i < Num_Boards; i++) {
        if (abs(j - i) >= level - 1) {
            drawSquare(textures[i], Projection * View * modelmat *
                                        GetPositionMatrix(
                                            0, 0, (i - j) * 0.45 / Num_Boards) *
                                        shearXX);
            drawSquare(
                textures[Num_Boards + i],
                Projection * View * modelmat * gen_rotation_deg(0, 90, 0) *
                    GetPositionMatrix(0, 0, (i - j) * 0.45 / Num_Boards) *
                    shearZZ);
        }
    }
}

struct Imposters {
    std::vector<glm::mat4> orientations;
    std::vector<glm::mat4> positions;
    std::vector<glm::mat4> scales;
    std::vector<int> levels;

    Imposters() {
        for (int i = 0; i < Num_Imposters; i++) {
            orientations.push_back(gen_rotation_deg(0, rand(), 0));
            double p = -rand() % (Num_Boards * Num_Forests);
            int l = ceil(abs(p) / (Num_Forests * 2));
            float rando = ((float)rand() / (RAND_MAX));
            if (rando > 0.33) {
                levels.push_back(l);
            }
            else {
                levels.push_back(Num_Boards / 2);
            }
            positions.push_back(GetPositionMatrix(rand() % 300 - 150, 0, p));
            scales.push_back(GetScaleMatrix(30, 30, 30));
        }
    }

    void render(GLuint* textures, double shearX, double shearZ) {
        for (int i = 0; i < Num_Imposters; i++) {
            drawImposterTree(&textures[i * 2 * Num_Boards],
                             positions[i] * orientations[i] * scales[i] *
                                 GetPositionMatrix(0.0, 1.0f, 0.0),
                             levels[i], shearX, shearZ);
        }
    }
};
