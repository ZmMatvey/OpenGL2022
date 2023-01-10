#include "screen.h"

#include "keyboard.h"
#include "mouse.h"

int screen::SCR_WIDTH = 0;
int screen::SCR_HEIGHT = 0;
bool screen::init_context_window = false;

void screen::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	//������ ���� �����������
	glfwMakeContextCurrent(window);

	//����� ��������� ������� ������������ ������������ ���� � ������� framebuffer
	glViewport(0, 0, width, height);

	screen::SCR_WIDTH = width;
	screen::SCR_HEIGHT = height;
}

void screen::OpenGL_Version(int major, int minor) {
	//OpenGL ������ 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	
	//��������� ������ ������� �� ������ 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

	//��� ������� �� Mac OS
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif //__APPLE__
}

void screen::Init_Glad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		throw ("Failed to initialize GLAD");
	}
}

screen::screen()
	: window(NULL) {}

void screen::init(int widht, int height, const char* name) {
	//������ ���� ���������������� ������� � ��������� OpenGL Project, 
	//4 �������� ��������� ��������� �� �������, 5 - ������� ���������� ������ ��������
	window = glfwCreateWindow(widht, height, name , NULL, NULL);

	if (window == NULL) {
		glfwTerminate();
		throw "Could not open window";
		return;
	}
	if (!init_context_window) {
		glfwMakeContextCurrent(window);
		init_context_window = true;
	}
}

void screen::MakeContextCurrent() {
	//������ ���� �����������
	glfwMakeContextCurrent(window);

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	framebufferSizeCallback(window, width, height);
}

GLFWwindow* screen::getWindow() const {
	return window;
}

int screen::getWidth() {
	return screen::SCR_WIDTH;
}

int screen::getHeight() {
	return screen::SCR_HEIGHT;
}

void screen::setFramebufferSize() const {
	//�������� ��������������� ������� �� ������� ���������� ����, ��� ��������� ������� ����
	glfwSetFramebufferSizeCallback(window, screen::framebufferSizeCallback);
}

void screen::setKeyboard() const {
	//�������� ��������������� ������� �� ������� ���������� ����� ������ � ���������� � ���� 
	glfwSetKeyCallback(window, keyboard::keyCallback);
}

void screen::setMouse(int mode_cursor) const {
	//������������ �������
	glfwSetInputMode(window, GLFW_CURSOR, mode_cursor);

	//���������� ����������
	glfwSetCursorPosCallback(window, mouse::cursorPosCallback); 
	glfwSetMouseButtonCallback(window, mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, mouse::mouseWheelCallback);
}

void screen::setParameters() const {
	setFramebufferSize();
	setKeyboard();
	setMouse();

	screen::setEnable();
}

void screen::setEnable() {
	glEnable(GL_DEPTH_TEST); //���� �������
}

void screen::setSize(int widht, int height) const {
	glfwSetWindowSize(window, widht, height);
}

void screen::setPos(int pos_x, int pos_y) const {
	glfwSetWindowPos(window, pos_x, pos_y);
}

void screen::update() const {
	//������� ���� �� ����, ��� ���� ���������� � ���������� ����� 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void screen::newFrame() const {
	//����(������� ��������������� ��� ���������!!!) ������ ������, ������ ������� �� �����, ����� �� ���� �������� ��� ����� ��������
	glfwSwapBuffers(window); 
}

bool screen::shouldClose() const {
	return glfwWindowShouldClose(window);
}

void screen::setShouldClose(bool shouldClose) const {
	glfwSetWindowShouldClose(window, shouldClose);
}

void screen::setFullScreen(int mode) const {
	glfwMaximizeWindow(window);
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void screen::setRestoreScreen(int mode) const {
	glfwRestoreWindow(window);
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}