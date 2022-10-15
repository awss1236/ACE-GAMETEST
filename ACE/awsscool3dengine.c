#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<glad/glad.h>
#include<GL/glfw3.h>

#include"awsscool3dengine.h"

int ACEInit(int width, int height){
	printf("Initializing A.C.E.\n");
	if(!glfwInit()){
		printf("Failure in the glfw wing\nAborting...\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GlobalACEApp.Window = glfwCreateWindow(width, height, "very cool ace app", NULL, NULL);

	glfwMakeContextCurrent(GlobalACEApp.Window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		printf("glad is not so glad anymore air or air\n");
	}
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	printf("Successfully inited A.C.E.\n");

	return 0;
}

bool ACEIsKeyDown(int key){
	return glfwGetKey(GlobalACEApp.Window, key) == GLFW_PRESS;
}

void ACELockMouse(){
	glfwSetInputMode(GlobalACEApp.Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Vec2 ACEMousePos(){
	double x, y;
	glfwGetCursorPos(GlobalACEApp.Window, &x, &y);
	return (Vec2){(float)x, (float)y};
}

void ACESetBGColor(float r, float g, float b){
	glClearColor(r, g, b, 1);
}

void ACEClearBG(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool ACEShouldQuit(){
	return glfwWindowShouldClose(GlobalACEApp.Window);
}

void ACEUpdateScreen(){
	glfwSwapBuffers(GlobalACEApp.Window);
	glfwPollEvents();
}

size_t SizeOfGLType(GLenum type){
	switch(type){
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			return sizeof(GLbyte);
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
			return sizeof(GLshort);
		case GL_INT_2_10_10_10_REV:
		case GL_INT:
		case GL_UNSIGNED_INT_2_10_10_10_REV:
		case GL_UNSIGNED_INT:
			return sizeof(GLint);
		case GL_FLOAT:
			return sizeof(GLfloat);
		case GL_DOUBLE:
			return sizeof(GLdouble);
		case GL_HALF_FLOAT:
			return sizeof(GLhalf);
	}
	return 0;
}


ACECamera ACEMakeCamera(Vec3 pos, Vec3 dir, Mat4 proj){
	return (ACECamera){pos, dir, proj};
}

void ACEUseCamera(ACECamera* cam){
	for(int i=0;i<GlobalACEApp.Scene->ObjectCount;i++){
		glUseProgram(GlobalACEApp.Scene->Objects[i]->Materiel.shader);
		SetShaderMat4Uniform(GlobalACEApp.Scene->Objects[i]->Materiel.shader, "Projection", cam->Projection);
	}
	GlobalACEApp.Camera = cam;
}

void ACEUseScene(ACEScene* scene){
	ACESetBGColor(scene->BGCol.x, scene->BGCol.y, scene->BGCol.z);
	GlobalACEApp.Scene = scene;
}

void ACERender(){
	ACEClearBG();

	Vec3 R = Vec3Normalize(Vec3CrossVec3(GlobalACEApp.Camera->Dir, (Vec3){0, 1, 0})),
		 U = Vec3Normalize(Vec3CrossVec3(GlobalACEApp.Camera->Dir, R));

	Mat4 view  = CreateViewMat4(R, U, GlobalACEApp.Camera->Dir, GlobalACEApp.Camera->Pos);

	for(int i=0;i<GlobalACEApp.Scene->ObjectCount;i++){
		glUseProgram(GlobalACEApp.Scene->Objects[i]->Materiel.shader);
		SetShaderMat4Uniform(GlobalACEApp.Scene->Objects[i]->Materiel.shader, "View", view);
		ACERenderObject(GlobalACEApp.Scene->Objects[i]);
	}

	free(view);

	ACEUpdateScreen();
}

void ACEQuit(){
	glfwTerminate();
}
