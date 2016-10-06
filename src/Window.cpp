#include "Window.h"

Window::Window()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	mWindow = glfwCreateWindow(1024, 768, "Arogine", NULL, NULL);
	if (mWindow == nullptr) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. We are sorry for that :/.\n");
		getchar();
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(mWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		exit(-1);
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, GL_TRUE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}

void Window::Update() const
{
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

GLFWwindow* Window::GetWindow()
{
	return mWindow;
}
