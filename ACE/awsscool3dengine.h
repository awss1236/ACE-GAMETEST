#include<stdbool.h>
#include<glad/glad.h>
#include<GL/glfw3.h>

#include"utils/utils.h"
#include"shaders/shader.h"
#include"acescene.h"

typedef struct{
	Vec3 Pos,
		 Dir;
	Mat4 Projection;
} ACECamera;

typedef struct{
	GLFWwindow* Window;
	ACEScene* Scene;
	ACECamera* Camera;
} ACE;

ACE GlobalACEApp;

int ACEInit(int width, int height);
bool ACEShouldQuit();
void ACEQuit();

bool ACEIsKeyDown(int key);
void ACELockMouse();
Vec2 ACEMousePos();

void ACESetBGColor(float r, float g, float b);
void ACEClearBG();
void ACEUpdateScreen();

ACECamera ACEMakeCamera(Vec3 pos, Vec3 dir, Mat4 proj);
void ACEUseCamera(ACECamera* cam);

void ACEUseScene(ACEScene* scene);

void ACERender();
