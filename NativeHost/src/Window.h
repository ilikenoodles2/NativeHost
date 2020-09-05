#pragma once

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

	void Init();
	void StartProcess(const bool& appReady, bool& windowInitialized);
	void Close() { m_IsOpen = false; }

	void SetContext(bool thisThread);
private:
	GLFWwindow* m_Window;
	bool m_IsOpen;
	OnUpdate m_OnUpdate;
};