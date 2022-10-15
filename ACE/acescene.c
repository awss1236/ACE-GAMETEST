#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#include"shaders/shader.h"
#include"aceprimitives.h"
#include"acescene.h"

ACEScene ACEMakeSceneEx(const char* defvert, const char* deffrag, float r, float g, float b, int MaxObjects){
	ACEScene out;
	out.BGCol = (Vec3){r, g, b};
	out.DefaultShader = CreateShaderProgram(defvert, deffrag);
	out.Objects = malloc(MaxObjects * sizeof(ACEObject*));
	out.ObjectCount = 0;
	out.MaxObjects = MaxObjects;
	return out;
}

ACEScene ACEMakeScene(float r, float g, float b, int MaxObjects){
	ACEScene out;
	out.BGCol = (Vec3){r, g, b};
	out.DefaultShader = CreateShaderProgram("ACE/shaders/defvert.glsl", "ACE/shaders/deffrag.glsl");
	out.Objects = malloc(MaxObjects * sizeof(ACEObject*));
	out.ObjectCount = 0;
	out.MaxObjects = MaxObjects;
	return out;
}

//The verformat takes the shape of an array of number-type pairs.
//Eg [3, GL_FLOAT, 2, GL_FLOAT] would specify two attribs (a vec3 and vec2).
//** That was the plan until I realised I'm dumb as shit and so everything is now a float.
//Eg [3, 2] would be the same as the prev example.
//
//Default format:
//		[3, 3, 2] 
//			3 vertex postion floats.
//			3 vertex normal floats.
//			2 texture coordinates.
//
//Default materiel:
//		- Simple white Color.
//		- Default Shader.

ACEMateriel ACEMakeColorMateriel(float r, float g, float b, ACEScene* scene){
	ACEMateriel out;

	out.Color = (Vec3){r, g, b};

	out.TextureCount = 0;
	out.shader = scene->DefaultShader;

	return out;
}

ACEObject ACEMakeObjectEx(float* vertices, int VerticesLength, int* vertformat, int FormatLength, ACEMateriel materiel, Mat4 model){
	static unsigned int id = 0;
	ACEObject out;
	out.ID = id++;
	//shader and Model
	out.Materiel = materiel;
	out.Model  = model;
	//that was the hard part.
	//now for the vertex array
	unsigned int vertarr, vertbuf;
	
	glGenVertexArrays(1, &vertarr);
	glGenBuffers(1, &vertbuf);

	glBindVertexArray(vertarr);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertbuf);
	glBufferData(GL_ARRAY_BUFFER, VerticesLength*sizeof(float), vertices, GL_STATIC_DRAW);

	int stride = 0;
	for(int i=0;i<FormatLength;i++){
		stride += vertformat[i];
	}
	stride *= sizeof(float);

	int offset = 0;

	for(int i=0;i<FormatLength;i++){
		glVertexAttribPointer(i, vertformat[i], GL_FLOAT, GL_FALSE, stride, (void*)offset);
		glEnableVertexAttribArray(i);
		offset += vertformat[i]*sizeof(float);
	}
	
	out.VertexArray = vertarr;
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Hardest part: Get VertexCount...
	out.VertexCount =  (sizeof(float) * VerticesLength) / stride;

	return out;
}

ACEObject ACEMakeSimpleObject(float* vertices, int VerticesLength, ACEScene* scene){
	ACEMateriel DefaultMateriel;

	DefaultMateriel.Color = (Vec3){1, 0, 0};

	DefaultMateriel.TextureCount = 0;
	DefaultMateriel.shader = scene->DefaultShader;

	return ACEMakeObjectEx(vertices, VerticesLength, (int [3]){3, 3, 2}, 3, DefaultMateriel, CreateMat4((float[16]){1, 0, 0, 0,
																													0, 1, 0, 0,
																													0, 0, 1, 0,
																													0, 0, 0, 1}));
}

ACEObject ACEMakePrimitive(ACEPrimitive prim, ACEScene* scene){
	switch(prim){
		case ACETRIG:
			return ACEMakeSimpleObject(TriangleVertices , TriangleLength, scene);
		case ACECUBE:
			return ACEMakeSimpleObject(CubeVertices, CubeLength, scene);
	}
}

bool ACEAddObject(ACEObject* obj, ACEScene* scene){
	if(scene->ObjectCount>=scene->MaxObjects){
		printf("TOO MANY FOKIN OBJECTS MAN.\n");
		return false;
	}
	scene->Objects[scene->ObjectCount++] = obj;
	return true;
}

bool ACERemoveObject(ACEObject* obj, ACEScene* scene){
	int ObjIdx = scene->MaxObjects;

	for(int i=0;i<scene->ObjectCount;i++){
		if(scene->Objects[i]->ID == obj->ID){
			ObjIdx = i;
			break;
		}
	}

	if(ObjIdx == scene->MaxObjects){
		printf("No such object in scene.\n");
		return false;
	}

	printf("%d, %d\n", ObjIdx, obj->ID);
	//Move everything after deleted object pointer back one pointer length
	void* to   = (void*)(scene->Objects[ObjIdx]);
	const void* from = (void*)(scene->Objects[ObjIdx + 1]);
	size_t num  = (scene->ObjectCount - ObjIdx - 1) * sizeof(ACEObject*);
	memcpy(to, from, num);

	scene->ObjectCount--;

	return true;
}

void ACERenderObject(ACEObject* obj){
	glBindVertexArray(obj->VertexArray);
	SetShaderMat4Uniform(obj->Materiel.shader, "Model", obj->Model);
	SetShaderVec3Uniform(obj->Materiel.shader, "Color", obj->Materiel.Color);
	glDrawArrays(GL_TRIANGLES, 0, obj->VertexCount);
}

