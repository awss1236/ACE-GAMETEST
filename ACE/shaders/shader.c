#include<stdlib.h>
#include<stdio.h>
#include<glad/glad.h>

#include"shader.h"

ShaderID CreateShaderFromPath(const char* path, unsigned int type){
	ShaderID shader = glCreateShader(type);
	const char* source = ReadFile(path);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	int success;
	char log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(shader, 512, NULL, log);
		printf("!!OMG T_T !! %s\n", log);
	}
	return shader;
}


void SetShaderMat4Uniform(ShaderID shader, const char* UniformName, Mat4 mat){
	glUniformMatrix4fv(glGetUniformLocation(shader, UniformName), 1, GL_TRUE, mat);
}

void SetShaderVec4Uniform(ShaderID shader, const char* UniformName, Vec4 vec){
	glUniform4fv(glGetUniformLocation(shader, UniformName),  1, &vec.x);
}

void SetShaderVec3Uniform(ShaderID shader, const char* UniformName, Vec3 vec){
	glUniform3fv(glGetUniformLocation(shader, UniformName),  1, &vec.x);
}

void SetShaderIntUniform(ShaderID shader, const char* UniformName, int i){
	glUniform1i(glGetUniformLocation(shader, UniformName), i);
}

void SetShaderFloatUniform(ShaderID shader, const char* UniformName, float f){
	glUniform1f(glGetUniformLocation(shader, UniformName), f);
}

ShaderID CreateShaderProgram(const char* vertpath, const char* fragpath){
	ShaderID vert = CreateShaderFromPath(vertpath, GL_VERTEX_SHADER);
	ShaderID frag = CreateShaderFromPath(fragpath, GL_FRAGMENT_SHADER);

	ShaderID program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);

	glDeleteShader(vert);
	glDeleteShader(frag);

	return program;
}
