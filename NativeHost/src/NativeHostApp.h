#pragma once

#include "Window.h"

#include "vendor/json.hpp"

class NativeHostApp
{
public:
	using OnTickFunc = void(*)(const nlohmann::json&, nlohmann::json&);

	NativeHostApp(OnTickFunc onMsg);
	~NativeHostApp();

	void Run();

	static NativeHostApp& Get() { return *s_Instance; }
private:
	Window m_Window;
	nlohmann::json m_InJSON, m_OutJSON;
	OnTickFunc m_OnMsg;

	static NativeHostApp* s_Instance;
private:
	bool ReadMsg();
	void SendMsg();
};

NativeHostApp* CreateNativeHostApp();