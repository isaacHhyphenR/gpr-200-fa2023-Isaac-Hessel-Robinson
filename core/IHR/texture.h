#pragma once
#include "../ew/external/glad.h"


namespace IHR {
	unsigned int loadTexture(const char* filePath, int WrapMode = GL_REPEAT, int minificationMode = GL_LINEAR_MIPMAP_LINEAR, int magnificationMode = GL_LINEAR);
}