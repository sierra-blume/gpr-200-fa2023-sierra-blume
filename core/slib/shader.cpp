#include "shader.h"

namespace slib {
	std::string loadShaderSourceFromFile(const std::string& filePath)
	{
		std::ifstream fstream(filePath);
		if (!fstream.is_open()) {
			printf("Failed to load file %s", filePath);
			return {};
		}
		std::stringstream buffer;
		buffer << fstream.rdbuf();
		return buffer.str();
	}

	unsigned int createrShader(GLenum shaderType, const char* sourceCode) {
		//Create the new shader object
		unsigned int shader = glCreateShader(shaderType);
		//Supply the shader object with source code
		glShaderSource(shader, 1, &sourceCode, NULL);
		//Compile the shader object
		glCompileShader(shader);
		//Checking for compile errors in vertex shaders
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			//512 is an arbitrary length, but should be plenty of characters for our error message.
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			printf("Failed to compile shader: %s", infoLog);
		}

		return shader;
	};

	unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
		//Create a new vertex shader object
		unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);

		//Create a new vertex shader object
		unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		unsigned int shaderProgram = glCreateProgram();
		//Attach each stage
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		//Link all the stages together
		glLinkProgram(shaderProgram);
		//Check for linking errors
		int success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			printf("Failed to link shader program: %s", infoLog);
		}
		//The linked program now contains our compiled code, so we can delete these intermediate objects
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	};


	//Shader class
	Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader) {
		std::string vertexShaderSource = loadShaderSourceFromFile(vertexShader.c_str());
		std::string fragmentShaderSource = loadShaderSourceFromFile(fragmentShader.c_str());
		m_id = createShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
	}

	void Shader::use() {
		glUseProgram(m_id);
	}

	void Shader::setInt(const std::string& name, int v) const
	{
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), v);
	}

	void Shader::setFloat(const std::string& name, float v) const
	{
		glUniform1f(glGetUniformLocation(m_id, name.c_str()), v);
	}

	void setVec2(const std::string& name, float x, float y) const
	{
		
	}

	void setVec3(const std::string& name, float x, float y, float z) const
	{

	}

	void setVec4(const std::string& name, float x, float y, float z, float w) const
	{

	}
}