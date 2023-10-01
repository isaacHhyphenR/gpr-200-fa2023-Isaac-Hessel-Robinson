#include "texture.h"
#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"



namespace IHR {
	unsigned int loadTexture(const char* filePath, int WrapMode, int minificationMode, int magnificationMode)
	{
		//image files stop top to bottom but UVs are bottom to top
		stbi_set_flip_vertically_on_load(true);

		int width, height, numComponents;
		//returns an array of bytes + width/height in pixel + number of components per pixel
		unsigned char* data = stbi_load(filePath, &width, &height, &numComponents, 0);

		if (data == NULL)
		{
			printf("Failed to load image %s", filePath);
			stbi_image_free(data);
			return 0;
		}

		unsigned int texture;
		glGenTextures(1, &texture); //creates new texture name
		glBindTexture(GL_TEXTURE_2D, texture); //binds texture to GL_TEXTURE_2D, so that it's a 2d texture

		GLint format = GL_RED;
		switch (numComponents)
		{
		case 1:
			format = GL_RED;
		case 2:
			format = GL_RG;
		case 3:
			format = GL_RGB;
		case 4:
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); //reserves memory and sets the data of texture

		//sets wrapping & scaling settings
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationMode);

		glGenerateMipmap(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
		return texture;
		
	}
}

