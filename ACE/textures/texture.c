#define STB_IMAGE_IMPLEMENTATION
#include"stbimage.h"
#include<stdlib.h>
#include<stdio.h>
#include<glad/glad.h>

#include"texture.h"

TextureID CreateTextureFromImage(const char* path){
	int w, h, channels;
	unsigned char* data = stbi_load(path, &w, &h, &channels, 0);

	if(!data){
		printf("Couldn't load texture\n");
		return 0;
	}

	TextureID texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	GLenum format;

	switch(channels){
		case 1:
			format = GL_LUMINANCE;
			break;
		case 2:
			format = GL_LUMINANCE_ALPHA;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	return texture;
}
