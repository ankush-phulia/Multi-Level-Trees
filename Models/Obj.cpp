#include <GL/glew.h>
#include <assimp/scene.h>  // Output data structure
#include <string.h>
#include <assimp/Importer.hpp>  // C++ importer interface
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

bool loadObjFile(std::string path, std::vector<unsigned short> &indices,
                 std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs,
                 std::vector<glm::vec3> &normals) {
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(path.c_str(), 0);
  if (!scene) {
    // fprintf( stderr, importer.GetErrorString());
    getchar();
    return false;
  }
  const aiMesh *mesh =
      scene->mMeshes[0];  // In this simple example code we always use the 1rst
                          // mesh (in OBJ files there is often only one anyway)

  // Fill vertices positions
  vertices.reserve(mesh->mNumVertices);
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    aiVector3D pos = mesh->mVertices[i];
    vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
  }

  // Fill vertices texture coordinates
  uvs.resize(mesh->mNumVertices);
  if (mesh->HasTextureCoords(0)) {
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      aiVector3D UVW =
          mesh->mTextureCoords[0][i];  // Assume only 1 set of UV coords; AssImp
                                       // supports 8 UV sets.
      uvs[i] = glm::vec2(UVW.x, UVW.y);
    }
  }

  // Fill vertices normals
  normals.reserve(mesh->mNumVertices);
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    aiVector3D n = mesh->mNormals[i];
    normals.push_back(glm::vec3(n.x, n.y, n.z));
  }

  // Fill face indices
  indices.reserve(3 * mesh->mNumFaces);
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    // Assume the model has only triangles.
    indices.push_back(mesh->mFaces[i].mIndices[0]);
    indices.push_back(mesh->mFaces[i].mIndices[1]);
    indices.push_back(mesh->mFaces[i].mIndices[2]);
  }

  // The "scene" pointer will be deleted automatically by "importer"
  return true;
}

struct objfile {
  std::vector<unsigned short> indices;
  std::vector<glm::vec3> indexed_vertices;
  std::vector<glm::vec2> indexed_uvs;
  std::vector<glm::vec3> indexed_normals;
  GLuint objvertexbuffer, objuvbuffer, objnormalbuffer, objelementbuffer;
  glm::vec3 color;
  float k_ambient, k_specular;

  objfile();
  objfile(std::string path, float r = 0.5, float g = 0.0, float b = 0.0,
          float ka = 0.3, float ks = 0.3) {
    bool res = loadObjFile(path, indices, indexed_vertices, indexed_uvs,
                           indexed_normals);
    color[0] = r;
    color[1] = g;
    color[2] = b;
    k_ambient = ka;
    k_specular = ks;

    glGenBuffers(1, &objvertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, objvertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3),
                 &indexed_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &objuvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, objuvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2),
                 &indexed_uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &objnormalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, objnormalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3),
                 &indexed_normals[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    glGenBuffers(1, &objelementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objelementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned short), &indices[0],
                 GL_STATIC_DRAW);
  }

  void disco(float r, float g, float b) {
    color[0] += r;
    color[1] += g;
    color[2] += b;
  }
};