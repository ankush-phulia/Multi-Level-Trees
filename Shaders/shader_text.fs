#version 330 core

in vec2 UV;

// Output data
out vec4 color;

uniform sampler2D myTextureSampler;

void main() {
	// Output color = red 
  color.rgb = texture( myTextureSampler, UV ).rgb;
  if (color.g<0.01) {
    discard;
  }
  color.a = 1.0;
}
