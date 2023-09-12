#include "shader.h"


namespace IHR {
	std::string loadShaderSourceFromFile(const std::string& filepath)
	{
		std::ifstream fstream(filepath);
		if (!fstream.is_open())
		{
			printf("Failed to load file %s", filepath);
			return{};
		}
		std::stringstream buffer;
		buffer << fstream.rdbuf();
		return buffer.str();
	}
}