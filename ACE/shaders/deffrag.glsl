#version 330 core

// This is the default fragment shader for A.C.E.
// This is kinda meant to be copy pasted and be built over as it uses the internal
// ACE uniform names.

uniform vec3 Color;
//uniform sampler2D TEXTURENAME		Add this for however many textures you have.

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

in vec3 FragPos;
in vec3 FragNorm;
in vec2 TexCoords;

void main(){

	float h = FragPos.y;

	gl_FragColor = vec4(vec3(0.2, 0.8, 0.5)/(mod(h, 0.1)*1/0.1), 1.);
}
