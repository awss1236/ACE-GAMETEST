#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include"utils.h"

const char* ReadFile(const char* path){
	FILE* fp;
	long size;
	char* out;
	fp = fopen(path, "rb");
	if(!fp){
		printf("Couldn't open file.\n");
		return out;
	}
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	out = calloc(1, size+1);
	if(!out){
		fclose(fp);
		printf("Buffer memory allocation failed while loading file\n");
		return out;
	}
	if(fread(out, size, 1, fp)!=1){
		fclose(fp);
		printf("fread() failed fsr");
		return out;
	}
	fclose(fp);
	return out;
}

void SetVec4(Vec4* v, float x, float y, float z, float w){
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}

void SetVec3(Vec3* v, float x, float y, float z){
	v->x = x;
	v->y = y;
	v->z = z;
}

float Vec4DotVec4(Vec4 a, Vec4 b){
	return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w;
}

Vec2 Vec2MultFloat(Vec2 v, float s){
	return (Vec2){v.x * s, v.y * s};
}

float Vec3Length(Vec3 v){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vec3 Vec3Normalize(Vec3 v){
	return Vec3MultFloat(v, 1/Vec3Length(v));
}

Vec3 Vec3CrossVec3(Vec3 a, Vec3 b){
	return (Vec3){a.y*b.z - a.z*b.y,
				  a.z*b.x - a.x*b.z,
				  a.x*b.y - a.y*b.x};
}

Vec3 Vec3MultFloat(Vec3 v, float s){
	return (Vec3){s*v.x,
				  s*v.y,
				  s*v.z};
}

Vec3 Vec3AddVec3(Vec3 a, Vec3 b){
	return (Vec3){a.x + b.x,
				  a.y + b.y,
				  a.z + b.z};
}

Vec3 Vec3RotX(Vec3 v, float a){
	return (Vec3){v.x, v.y*cos(a) - v.z*sin(a), v.y*sin(a)+v.z*cos(a)};
}

Vec3 Vec3RotY(Vec3 v, float a){
	return (Vec3){v.x*cos(a)+v.z*sin(a), v.y, -v.x*sin(a)+v.z*cos(a)};
}

Vec3 Vec3RotZ(Vec3 v, float a){
	return (Vec3){v.x*cos(a)-v.y*sin(a), v.x*sin(a)+v.y*cos(a), v.z};
}

Mat4 CreateMat4(float init[16]){
	Mat4 out = (float*)malloc(16 * sizeof(float));
	for(int i=0;i<16;i++){
		out[i] = init[i];
	}
	return out;
}

Mat4 CreateProjMat4(float n, float f, float right, float top){
   return CreateMat4((float[16]){n/right,		  0,			0,			  0,
								 0,		   	  n/top,			0,			  0,
								 0,				  0, -(f+n)/(f-n), -2*f*n/(f-n),
								 0,				  0,		   -1,			  0});
}

Mat4 CreateViewMat4(Vec3 right, Vec3 up, Vec3 dir, Vec3 pos){
	Mat4 mat1 = CreateMat4((float[16]){right.x, right.y, right.z, 0,
									   up.x	  , up.y   , up.z	, 0,
									  -dir.x  ,-dir.y  ,-dir.z	, 0,
									   0	  , 0	   , 0		, 1}),
		 mat2 = CreateMat4((float[16]){1, 0, 0, -pos.x,
				 					   0, 1, 0, -pos.y,
									   0, 0, 1, -pos.z,
									   0, 0, 0, 1});
	Mat4 result = Mat4MultMat4(mat1, mat2);
	free(mat1);
	free(mat2);
	return result;
}

Mat4 RotXMat4(float a){
	return CreateMat4((float[16]){1,	  0,	   0, 0, 
							   	  0, cos(a), -sin(a), 0,
							   	  0, sin(a),  cos(a), 0,
								  0,	  0,	   0, 1});
}
Mat4 RotYMat4(float a){
	return CreateMat4((float[16]){cos(a), 0, sin(a), 0, 
								  	   0, 1, 	  0, 0,
								 -sin(a), 0, cos(a), 0,
								  	   0, 0,	  0, 1});
}
Mat4 RotZMat4(float a){
	return CreateMat4((float[16]){cos(a), -sin(a), 0, 0, 
								  sin(a),  cos(a), 0, 0,
									   0,		0, 1, 0,
								  	   0,		0, 0, 1});
}

void Mat4RotX(Mat4* m, float a){
	Mat4 rot = RotXMat4(a);
	Mat4 out = Mat4MultMat4(*m, rot);
	free(rot);
	free(*m);
	*m = out;
}
void Mat4RotY(Mat4* m, float a){
	Mat4 rot = RotYMat4(a);
	Mat4 out = Mat4MultMat4(*m, rot);
	free(rot);
	free(*m);
	*m = out;
}
void Mat4RotZ(Mat4* m, float a){
	Mat4 rot = RotZMat4(a);
	Mat4 out = Mat4MultMat4(*m, rot);
	free(rot);
	free(*m);
	*m = out;
}

void Mat4Trans(Mat4* m, float x, float y, float z){
	Mat4 T = CreateMat4((float[16]){1, 0, 0, x,
									0, 1, 0, y,
									0, 0, 1, z,
									0, 0, 0 ,1});
	Mat4 out= Mat4MultMat4(*m, T);
	free(T);
	free(*m);
	*m = out;
}

void Mat4Scale(Mat4* m, float x, float y, float z){
	Mat4 T = CreateMat4((float[16]){x, 0, 0, 0,
									0, y, 0, 0,
									0, 0, z, 0,
									0, 0, 0 ,1});
	Mat4 out= Mat4MultMat4(*m, T);
	free(T);
	free(*m);
	*m = out;
}

Vec4 Vec4MultMat4(Vec4 v, Mat4 m){
	Vec4 out;
	out.x = v.x*m[0] + v.y*m[1] + v.z*m[2] + v.w*m[3];
	out.y = v.x*m[4] + v.y*m[5] + v.z*m[6] + v.w*m[7];
	out.z = v.x*m[8] + v.y*m[9] + v.z*m[10]+ v.w*m[11];
	out.w = v.x*m[12]+ v.y*m[13]+ v.z*m[14]+ v.w*m[15];
	return out;
}

void PrintVec2(Vec2 v){
	printf("%.2f, %.2f\n", v.x, v.y);
}

void PrintVec3(Vec3 v){
	printf("%.2f, %.2f, %.2f\n", v.x, v.y, v.z);
}

void PrintVec4(Vec4 v){
	printf("%.2f, %.2f, %.2f, %.2f\n", v.x, v.y, v.z, v.w);
}

void PrintMat4(Mat4 m){
	printf("--------------------\n");
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			printf("%.2f|", m[j+i*4]);
		}
		printf("\n--------------------\n");
	}
}

Mat4 Mat4MultMat4(Mat4 a, Mat4 b){
	Mat4 out = CreateMat4((float[16]){0});
	for(int i=0;i<16;i++){
		Vec4 ARow, BCol;
		ARow   = (Vec4){a[0 + 4*(i/4)], a[1 + 4*(i/4)], a[2 + 4*(i/4)], a[3 + 4*(i/4)]};
		BCol   = (Vec4){b[0 + i%4], b[4 + i%4], b[8 + i%4], b[12+ i%4]};
		out[i] = Vec4DotVec4(ARow, BCol);
	}
	return out;
}
