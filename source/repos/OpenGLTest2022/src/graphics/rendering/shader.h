#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream> 

#define GLFW_INCLUDE_NONE
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class shader final 
{
private:
	unsigned int id;

	bool copy;

public:
	unsigned int get_id() const;

	shader();
	shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	
	shader(const shader& a);
	shader& operator=(const shader& a);
	~shader();

	void generate(const char* vertexShaderPath, const char* fragmentShaderPath);

	void activate() const;

	std::string loadShaderSrc(const char* filename) const;
	GLuint compileShader(const char* filepath, GLenum type) const;

	void setMat3(const std::string& name, glm::mat3 value) const;
	void setMat4(const std::string& name, glm::mat4 val) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

#endif //SHADER_H

