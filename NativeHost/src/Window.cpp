#include "Window.h"

static void ErrorCallback(int error, const char* description)
{
	s_Logfile << "Error: " << error << description << std::endl;
}

static void WindowCloseCallback(GLFWwindow* window)
{
	Window* windowPtr = (Window*)glfwGetWindowUserPointer(window);
	glfwDestroyWindow(windowPtr->Get());
	glfwTerminate();

	windowPtr->~Window();
}

Window::Window()
{
	glfwSetErrorCallback(ErrorCallback);

	if (!glfwInit())
		s_Logfile << "Failed to initialize GLFW" << std::endl;

	m_Window = glfwCreateWindow(640, 480, "Native host", NULL, NULL);
	if (!m_Window)
	{
		glfwTerminate();
		s_Logfile << "Failed to create window" << std::endl;
		return;
	}

	glfwSetWindowUserPointer(m_Window, this);
	glfwSetWindowCloseCallback(m_Window, WindowCloseCallback);

	m_IsOpen = true;
	s_Logfile << "Successfully created window" << std::endl;
}

Window::~Window()
{
	m_IsOpen = false;
}

void Window::StartProcess()
{
	while (m_IsOpen)
		glfwPollEvents();
}
