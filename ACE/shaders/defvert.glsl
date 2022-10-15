#version 330 core

// This is the default vertex shader for A.C.E.
// This is kinda meant to be copy pasted and be built over as it uses the internal
// ACE uniform names.

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTex;

uniform mat4 Model, View, Projection;

out vec3 FragPos;
out vec3 FragNorm;
out vec2 TexCoords;

void main(){
	FragPos = vec3(Model * vec4(aPos, 1.));
	FragNorm = aNorm;
	TexCoords = aTex;
	gl_Position = Projection * View * Model * vec4(aPos, 1.);
}
