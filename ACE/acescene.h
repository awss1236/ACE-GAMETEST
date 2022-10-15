#include<stdbool.h>
#include<glad/glad.h>
#include<GL/glfw3.h>

#include"utils/utils.h"

typedef enum{
	ACETRIG,
	ACECUBE
} ACEPrimitive;

#define MAXSCENEOBJECTS 5

typedef struct{
	Vec3 Color;
	unsigned int* Textures, 
				  TextureCount,
				  shader;
} ACEMateriel;

typedef struct{
	unsigned int VertexArray,
				 VertexCount,
				 ID;
	ACEMateriel Materiel;
	Mat4 Model;
} ACEObject;

typedef struct{
	Vec3 BGCol;
	unsigned int DefaultShader;
	ACEObject** Objects;
	int ObjectCount,
		MaxObjects;
} ACEScene;

ACEScene ACEMakeSceneEx(const char* defvert, const char* deffrag, float r, float g, float b, int MaxObjects);
ACEScene ACEMakeScene(float r, float g, float b, int MaxObjects);

ACEMateriel ACEMakeColorMateriel(float r, float g, float b, ACEScene* scene);
ACEObject ACEMakeObjectEx(float* vertices, int VerticesLength, int* vertformat, int FormatLength, ACEMateriel materiel, Mat4 model);
ACEObject ACEMakeSimpleObject(float* vertices, int VerticesLength, ACEScene* scene);
ACEObject ACEMakePrimitive(ACEPrimitive prim, ACEScene* scene);
bool ACEAddObject(ACEObject* obj, ACEScene* scene);
bool ACERemoveObject(ACEObject* obj, ACEScene* scene);
void ACERenderObject(ACEObject* obj);
