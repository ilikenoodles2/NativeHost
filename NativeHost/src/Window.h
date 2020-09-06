#pragma once

#include "Logger.h"

#include "vendor/glfw/glfw/include/GLFW/glfw3.h"

#include <fstream>

// Used to log everything into a file, because we are 
// unable to use the standard cout
static std::ofstream s_Logfile = std::ofstream("NativeLog.txt");

class Window 
{
public:
	using OnUpdate = void(*)(const float);

	Window(OnUpdate onUpdate);
	~Window();

	void StartProcess(const bool& appReady, bool& windowInitialized);
	void Close() { m_IsOpen = false; }

	void SetContext(bool thisThread);
	Logger* GetLogger() { return m_Logger; }
private:
	GLFWwindow* m_Window;
	bool m_IsOpen;
	OnUpdate m_OnUpdate;

	Logger* m_Logger;
};