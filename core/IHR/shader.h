#pragma once

#include <sstream>
#include <fstream>

namespace IHR {
	std::string loadShaderSourceFromFile(const std::string& filepath);

	class Shader {
	public:
		Shader(const std::string& vertexShader, const std::string& fragmentShader);
		void use();
	};
}