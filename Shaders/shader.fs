#version 330 core

in vec3 fragCol;

// Ouput data
out vec3 color;

void main() {
	// Output color = red 
	color = fragCol;
}
