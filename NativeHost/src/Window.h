#pragma once

#include "vendor/glfw/glfw/include/GLFW/glfw3.h"

#include <fstream>

// Used to log everything into a file, because we are 
// unable to use the standard cout
static std::ofstream s_Logfile = std::ofstream("NativeLog.txt");

class Window 
{
public:
	Window();
	~Window();

	void StartProcess();

	GLFWwindow* Get() { return m_Window; }
private:
	GLFWwindow* m_Window;
	bool m_IsOpen;
};