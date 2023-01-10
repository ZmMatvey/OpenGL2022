#include "Shader.h"

unsigned int shader::get_id() const {
	return id;
}

shader::shader() : id(0), copy(true) {}
shader::shader(const char* vertexShaderPath, const char* fragmentShaderPath) : copy(false) {
	generate(vertexShaderPath, fragmentShaderPath);
}

shader::shader(const shader& a) : id(a.id), copy(true) {}

shader& shader::operator=(const shader& a) {
	shader b;
	b.id = a.id;
	b.copy = true;
	return b;
}

void shader::generate(const char* vertexShaderPath, const char* fragmentShaderPath) {
	int success;
	char infoLog[512];

	GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragShader);
	glLinkProgram(id);

	//catch error
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "Linking error:" << std::endl << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

void shader::activate() const {
	glUseProgram(id);
}

std::string shader::loadShaderSrc(const char* filename) const {
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	file.open(filename);

	if (file.is_open()) {
		buf << file.rdbuf(); 
		ret = buf.str();
	}
	else {
		std::cout << "error open " << filename << std::endl;
		throw(std::logic_error("error open"));
	}

	file.close();

	return ret;
}

GLuint shader::compileShader(const char* filepath, GLenum type) const {
	int success;
	char infoLog[512];

	GLuint ret = glCreateShader(type); //������ ������ ��� ������� ���� �������
	std::string shaderSrc = loadShaderSrc(filepath); //��������� ������ � ������
	const GLchar* shader = shaderSrc.c_str(); //��������� ������ � ������ const char

	//1 - � ���� �����������, 2 - ���������� �����, 3 - ��� ������� const char, 4 - ������ ���� �����
	glShaderSource(ret, 1, &shader, NULL); //����������� ������ � ���������������� �������
	glCompileShader(ret); //����������� ������ � ����������

	//catch error
	//1 - id �������, 2 - �� ����� ������ ���������� ����� ���������, 3 - ��������� 1 ���� �������� ���� ���������, ����� 0
	glGetShaderiv(ret, GL_COMPILE_STATUS, &success); 
	if (!success) {
		//1 - id �������, 2 - ������������ ������ ���� ������, 3 - ��������� ������ ���� ������, 4 - ���� ������������ ��� ������
		glGetShaderInfoLog(ret, 512, NULL, infoLog);
		std::cout << "Error with shader("<< type <<") compile: " << std::endl << infoLog << std::endl;
		throw(std::logic_error("Error with shader compile"));
	}

	return ret;
}

shader::~shader() {
	if (!copy) {
		glDeleteProgram(id);
	}
}

//�������� ������� name � vertex_shader(id), ��� uniform
void shader::setMat3(const std::string& name, glm::mat3 value) const {
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void shader::setMat4(const std::string& name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
//�������� int name
void shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
//�������� float name
void shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}