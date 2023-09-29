#include "texture.h"
#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"



unsigned int loadTexture(const char* filePath)
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
}

