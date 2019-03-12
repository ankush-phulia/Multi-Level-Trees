#include <glm/glm.hpp>

glm::mat4 GetScaleMatrix(float s1, float s2, float s3) {
    glm::mat4 ans = glm::mat4(1.0f);
    ans[0][0] = s1;
    ans[1][1] = s2;
    ans[2][2] = s3;
    return ans;
}

glm::mat4 GetPositionMatrix(float s1, float s2, float s3) {
    glm::mat4 ans = glm::mat4(1.0f);
    ans[3][0] = s1;
    ans[3][1] = s2;
    ans[3][2] = s3;
    return ans;
}

glm::mat4 GetRotationMatrixZ(float theta) {
    glm::mat4 ans = glm::mat4(1.0f);
    ans[0][0] = cos(theta);
    ans[1][0] = sin(theta);
    ans[0][1] = -sin(theta);
    ans[1][1] = cos(theta);
    return ans;
}

glm::mat4 GetRotationMatrixY(float theta) {
    glm::mat4 ans = glm::mat4(1.0f);
    ans[0][0] = cos(theta);
    ans[2][0] = -sin(theta);
    ans[0][2] = sin(theta);
    ans[2][2] = cos(theta);
    return ans;
}

glm::mat4 GetRotationMatrixX(float theta) {
    glm::mat4 ans = glm::mat4(1.0f);
    ans[1][1] = cos(theta);
    ans[1][2] = -sin(theta);
    ans[2][1] = sin(theta);
    ans[2][2] = cos(theta);
    return ans;
}

glm::mat4 GetShearMatrixX(float theta) {
    glm::mat4 ans = glm::mat4(1.0f);
    ans[0][0] = 1.0;
    ans[1][1] = 1.0;
    ans[2][2] = 1.0;
    ans[0][1] = theta;
    return ans;
}

glm::mat4 gen_rotation(float x, float y, float z) {
    glm::mat4 ans = glm::mat4(1.0f);
    ans = GetRotationMatrixX(x) * GetRotationMatrixY(y) * GetRotationMatrixZ(z);
    return ans;
}

glm::mat4 gen_rotation_deg(float x, float y, float z) {
    return gen_rotation(x * M_PI / 180.0, y * M_PI / 180.0, z * M_PI / 180.0);
}

glm::mat4 GetTranslationMatrix(float x, float y, float z) {
    glm::mat4 ans = glm::mat4(1.0f);
    ans[3][0] = x;
    ans[3][1] = y;
    ans[3][2] = z;
    return ans;
}
