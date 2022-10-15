#ifndef UTILS_H
#define UTILS_H
const char* ReadFile(const char* path);

typedef struct{
	float x, y;
}Vec2;

typedef struct{
	float x, y, z;
}Vec3;

typedef struct{
	float x, y, z, w;
}Vec4;

typedef float* Mat4;

void PrintVec2(Vec2 v);
void PrintVec3(Vec3 v);
void PrintVec4(Vec4 v);
void PrintMat4(Mat4 m);

//implement shit as you need it is tha plan

void SetVec4(Vec4* v, float x, float y, float z, float w);
void SetVec3(Vec3* v, float x, float y, float z);

float Vec4DotVec4(Vec4 a, Vec4 b);

Vec2 Vec2MultFloat(Vec2 v, float s);

float Vec3Length(Vec3 v);
Vec3 Vec3Normalize(Vec3 v);
Vec3 Vec3MultFloat(Vec3 v, float s);
Vec3 Vec3AddVec3(Vec3 a, Vec3 b);
Vec3 Vec3CrossVec3(Vec3 a, Vec3 b);
Vec3 Vec3RotX(Vec3 v, float a);
Vec3 Vec3RotY(Vec3 v, float a);
Vec3 Vec3RotZ(Vec3 v, float a);

Mat4 CreateMat4(float init[16]);
Mat4 CreateProjMat4(float near, float far, float right, float top);
Mat4 CreateViewMat4(Vec3 right, Vec3 up, Vec3 dir, Vec3 pos);

void Mat4RotX(Mat4* m, float angle);
void Mat4RotY(Mat4* m, float angle);
void Mat4RotZ(Mat4* m, float angle);
void Mat4Trans(Mat4* m, float x, float y, float z);
void Mat4Scale(Mat4* m, float x, float y, float z);
void Mat4PresProj(Mat4* m, float AR, float fov, float NearZ, float FarZ);

Vec4 Vec4MultMat4(Vec4 v, Mat4 m);
Mat4 Mat4MultMat4(Mat4 a, Mat4 b);
#endif
