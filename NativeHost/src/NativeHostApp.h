#pragma once

#include "Window.h"

#include "vendor/json.hpp"

#include <thread>

class NativeHostApp
{
public:
	using OnTickFunc = void(*)(const nlohmann::json&, nlohmann::json&);

	NativeHostApp(Window::OnUpdate onUpdate, OnTickFunc onMsg);
	~NativeHostApp();

	void Run();

	static NativeHostApp& Get() { return *s_Instance; }
	Logger* GetLogger() { return m_Window.GetLogger(); }
private:
	bool m_Running = false;
	Window m_Window;
	std::thread m_WindowProcess;

	nlohmann::json m_InJSON, m_OutJSON;
	OnTickFunc m_OnMsg;

	static NativeHostApp* s_Instance;
private:
	bool ReadMsg();
	void SendMsg();
};

NativeHostApp* CreateNativeHostApp();