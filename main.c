#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include"noise.h"
#include"ACE/awsscool3dengine.h"

#define GRIDWIDTH  100
#define GRIDHEIGHT 100

ACEScene scn;
ACECamera cam;

Vec2 PMouse;

float pitch = 1.57, yaw = 0;

void ProcessInput(){

	const float MSens = 0.01;
	const float MoveS = 0.002;

	if(ACEIsKeyDown(GLFW_KEY_W)){
		cam.Pos = Vec3AddVec3(cam.Pos, Vec3MultFloat(cam.Dir, MoveS));
	}else if(ACEIsKeyDown(GLFW_KEY_S)){
		cam.Pos = Vec3AddVec3(cam.Pos, Vec3MultFloat(cam.Dir,-MoveS));
	}

	Vec3 R = Vec3Normalize(Vec3CrossVec3(cam.Dir, (Vec3){0, 1, 0}));

	if(ACEIsKeyDown(GLFW_KEY_D)){
		cam.Pos = Vec3AddVec3(cam.Pos, Vec3MultFloat(R, MoveS));
	}else if(ACEIsKeyDown(GLFW_KEY_A)){
		cam.Pos = Vec3AddVec3(cam.Pos, Vec3MultFloat(R,-MoveS));
	}

	Vec2 CMouse = ACEMousePos();

	float dx = CMouse.x - PMouse.x,
		  dy = CMouse.y - PMouse.y;

	PMouse = CMouse;

	yaw += dx*MSens;
	pitch += dy*MSens;

	pitch = fmax(fmin(pitch, 1.57), -1.57);

	cam.Dir.x = cos(yaw)*cos(pitch);
	cam.Dir.y = sin(pitch);
	cam.Dir.z = sin(yaw)*cos(pitch);

}

int main(int argc, char* argv[]){
	ACEInit(600, 600);
	ACELockMouse();

	scn = ACEMakeScene(0.5, 0.8, 1, 5);
	ACEUseScene(&scn);

	ACEObject world;
	float* mesh = malloc(8*6*(GRIDWIDTH-1)*GRIDHEIGHT*sizeof(float));
	
	//dumb mesh generation that works i guess.
	for(int j=0;j<GRIDHEIGHT;j++){
		for(int i=0;i<6*(GRIDWIDTH-1);i++){

			int ti  = i/2;

			float x = (ti/3)+ti%3,
				  z = j+i%2;

			mesh[8*(j*6*(GRIDWIDTH-1)+i) + 0] = x/16;
			mesh[8*(j*6*(GRIDWIDTH-1)+i) + 1] = Noise2D(x, z, 0.05, 16);
			mesh[8*(j*6*(GRIDWIDTH-1)+i) + 2] = z/16;

			mesh[8*(j*6*(GRIDWIDTH-1)+i) + 6] = i%3;
			mesh[8*(j*6*(GRIDWIDTH-1)+i) + 7] = i%2;
		}
	}

	world = ACEMakeSimpleObject(mesh, 8*6*(GRIDWIDTH-1)*GRIDHEIGHT, &scn);
	ACEAddObject(&world, &scn);

	free(mesh);

	ACEObject c = ACEMakePrimitive(ACECUBE, &scn);
	//ACEAddObject(&c, &scn);
	c.Materiel = ACEMakeColorMateriel(0.2, 0.5, 0.1, &scn);

	cam = ACEMakeCamera((Vec3){0, 0, 0}, (Vec3){0, 0, 1}, CreateProjMat4(0.001, 100, 0.001, 0.001));
	ACEUseCamera(&cam);

	Mat4Trans(&c.Model, 0, 0, 2);

	PMouse = ACEMousePos();	
	bool quit = false;
	while(!(ACEShouldQuit() || quit)){
		ProcessInput();
		cam.Pos.y = Noise2D(16*cam.Pos.x, 16*cam.Pos.z, 0.05, 16) - 0.05;
		ACERender();

		if(ACEIsKeyDown(GLFW_KEY_ESCAPE)){
			quit = true;
		}
	}
	ACEQuit();
	return 0;
}
