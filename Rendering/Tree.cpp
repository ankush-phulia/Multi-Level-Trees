#include "Grammar.cpp"
#include "Manipulate.cpp"
#include "../Models/Obj.cpp"

#define DegreeToRadiun 0.0174533

glm::mat4 Projection, View;
GLuint programObj;
GLuint ObjMatrixID, ObjViewMatrixID, ObjModelMatrixID;
GLuint ObjTexture, ObjTextureID;

void drawObj(objfile &f, glm::mat4 ModelMatrix = glm::mat4(1.0f)) {
  glUseProgram(programObj);
  GLuint LightID = glGetUniformLocation(programObj, "LightPosition_worldspace");
  GLuint ColorID = glGetUniformLocation(programObj, "SolidColor");
  GLuint Lighting = glGetUniformLocation(programObj, "Lighting");

  glm::mat4 MVP = Projection * View * ModelMatrix;
  glUniformMatrix4fv(ObjMatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(ObjModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
  glUniformMatrix4fv(ObjViewMatrixID, 1, GL_FALSE, &View[0][0]);

  glm::vec3 lightPos = glm::vec3(0, 0, 10);
  glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
  glUniform3f(ColorID, f.color.x, f.color.y, f.color.z);
  glUniform2f(Lighting, f.k_ambient, f.k_specular);

  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ObjTexture);
  // Set our "myTextureSampler" sampler to user Texture Unit 0
  glUniform1i(ObjTextureID, 0);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, f.objvertexbuffer);
  glVertexAttribPointer(0,         // attribute
                        3,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void *)0  // array buffer offset
  );

  // 2nd attribute buffer : UVs
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, f.objuvbuffer);
  glVertexAttribPointer(1,         // attribute
                        2,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void *)0  // array buffer offset
  );

  // 3rd attribute buffer : normals
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, f.objnormalbuffer);
  glVertexAttribPointer(2,         // attribute
                        3,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void *)0  // array buffer offset
  );

  // Index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, f.objelementbuffer);

  // Draw the triangles !
  glDrawElements(GL_TRIANGLES,       // mode
                 f.indices.size(),   // count
                 GL_UNSIGNED_SHORT,  // type
                 (void *)0           // element array buffer offset
  );

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

class Tree {
  std::string Tree_string;
  std::unordered_map<char, glm::mat4> rotations;
  grammar g;
  std::unordered_map<int, glm::mat4> bone_mats;
  glm::mat4 leafmat;
  glm::mat4 position;

 public:
  glm::mat4 orientation;

  Tree(glm::mat4 p, glm::mat4 o) {
    g.insert("B",
             "F[dFl[Bl]][eFl[lB]][aFla[BlFl]][cFl[cFlacl[Bl]]][bFl[bFla[Bl]]l]",
             0.12);
    g.insert("B",
             "F[eFl[Bl]][dFl[lB]][bFlb[BlFl]][cFl[cFlbcl[Bl]]][aFl[aFlb[Bl]]l]",
             0.12);
    g.insert(
        "B",
        "F[dFl[Bl]][eFl[lB]][cFlc[BlFl]][aFl[aFl[Bl]cal[Bl]]][bFl[bFlc[Bl]]l]",
        0.12);
    g.insert(
        "B",
        "F[dFl[lB]][eFl[Bl]][aFl[Bl]a[FlBl]][cFl[cFlacl[lB]]][bFl[bFla[lB]]l]",
        0.12);
    g.insert("B",
             "F[dFl[Bl]][eFl[lB]][aFl[Bl]a[BlFl]][cFl[cFl[Bl]acl[Bl]]][bFl[bFl["
             "Bl]a[Bl]]l]",
             0.52);
    g.insert("a", "a");
    g.insert("b", "b");
    g.insert("c", "c");
    g.insert("d", "d");
    g.insert("e", "e");
    g.insert("l", "l");
    g.insert("[", "[");
    g.insert("]", "]");
    g.insert("F", "F");

    double angle1 = rand() % 30 - 15;
    double angle2 = rand() % 30 - 15;
    double angle3 = rand() % 30 - 15;
    double angle4 = rand() % 30 - 15;
    double angle5 = rand() % 30 - 15;
    rotations['a'] = gen_rotation((15.0 + angle1) * DegreeToRadiun,
                                  (30.0 + angle1) * DegreeToRadiun,
                                  (-20.0 + angle1) * DegreeToRadiun);
    rotations['b'] = gen_rotation((20.0 + angle2) * DegreeToRadiun,
                                  (20.0 + angle2) * DegreeToRadiun,
                                  (10.0 + angle2) * DegreeToRadiun);
    rotations['c'] = gen_rotation((-15.0 + angle3) * DegreeToRadiun,
                                  (20.0 + angle3) * DegreeToRadiun,
                                  (20.0 + angle3) * DegreeToRadiun);
    rotations['d'] = gen_rotation((-10.0 + angle4) * DegreeToRadiun,
                                  (-10 + angle4) * DegreeToRadiun,
                                  (0.0 + angle4) * DegreeToRadiun);
    rotations['e'] = gen_rotation((-10.0 + angle5) * DegreeToRadiun,
                                  (20 + angle5) * DegreeToRadiun,
                                  (10.0 + angle5) * DegreeToRadiun);

    position = p;
    orientation = o;
    leafmat =
        GetScaleMatrix(0.5, 0.5, 0.5) * GetPositionMatrix(0.0, 0.0, -2.175);
  };

  void gen_string(std::string start, int age) {
    Tree_string = g.gen(age, start);
  }

  glm::mat4 get_boneMatrix(int f2) {
    if (bone_mats.find(f2) != bone_mats.end()) {
      return bone_mats[f2];
    }
    float t = 1.0;
    for (int i = 1; i < f2; i++) {
      t *= 0.8;
    }
    glm::mat4 sc = GetScaleMatrix(t, t, t);
    bone_mats[f2] = sc;
    return sc;
  }

  void render(objfile Leaf_Object, objfile Branch_Object) {
    std::vector<glm::mat4> rotation_stack;
    std::vector<glm::mat4> position_stack;

    rotation_stack.push_back(gen_rotation(0, 0, 0));
    position_stack.push_back(gen_rotation(0, 0, 0));

    for (auto ch : Tree_string) {
      if (ch == 'F') {
        glm::mat4 modelmat = position * orientation * position_stack.back() *
                             rotation_stack.back() *
                             get_boneMatrix(position_stack.size()) *
                             GetPositionMatrix(0, 5, 0);
        drawObj(Branch_Object, modelmat);
        position_stack[position_stack.size() - 1][3][0] +=
            10.0 * get_boneMatrix(position_stack.size())[1][1] *
            rotation_stack[rotation_stack.size() - 1][1][0];
        position_stack[position_stack.size() - 1][3][1] +=
            10.0 * get_boneMatrix(position_stack.size())[1][1] *
            rotation_stack[rotation_stack.size() - 1][1][1];
        position_stack[position_stack.size() - 1][3][2] +=
            10.0 * get_boneMatrix(position_stack.size())[1][1] *
            rotation_stack[rotation_stack.size() - 1][1][2];
      } else if (ch == '[') {
        position_stack.push_back(position_stack[position_stack.size() - 1]);
        rotation_stack.push_back(rotation_stack[rotation_stack.size() - 1]);
      } else if (ch == ']') {
        position_stack.pop_back();
        rotation_stack.pop_back();
      } else if (ch == 'a' or ch == 'b' or ch == 'c' or ch == 'd' or
                 ch == 'e') {
        rotation_stack[rotation_stack.size() - 1] *= rotations[ch];
      } else if (ch == 'l') {
        glm::mat4 translation = position_stack.back() * rotation_stack.back();
        glm::mat4 modelmat = position * orientation * translation * leafmat;
        float rando = ((float)rand() / (RAND_MAX)) - 0.5;
        rando /= 8;
        Leaf_Object.disco(15 * rando, -rando * 8, rando * 5);
        drawObj(Leaf_Object, modelmat);
      }
    }
  }
};