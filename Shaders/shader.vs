#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexCol;

out vec3 fragCol;

// Values that stay constant for the whole mesh.
uniform mat4 MVP1;

void main(){
	gl_Position = MVP1*vec4(vertexPosition_modelspace, 1);
	fragCol = vertexCol;
}
