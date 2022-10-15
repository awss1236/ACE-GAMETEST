#include"..\utils\utils.h"

typedef unsigned int ShaderID;

ShaderID CreateShaderFromPath(const char* path, unsigned int type);
ShaderID CreateShaderProgram(const char* vertpath, const char* fragpath);

//These assume you use the shader before changin the uniforms
void SetShaderMat4Uniform(ShaderID shader, const char* UniformName, Mat4 mat);
void SetShaderVec4Uniform(ShaderID shader, const char* UniformName, Vec4 vec);
void SetShaderVec3Uniform(ShaderID shader, const char* UniformName, Vec3 vec);
void SetShaderIntUniform(ShaderID shader,  const char* UniformName, int i);
void SetShaderFloatUniform(ShaderID shader,const char* UniformName, float f);
