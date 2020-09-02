#pragma once

#include "vendor/json.hpp"

#include <sstream>
#include <fstream>

class NativeHostApp
{
public:
	using OnTickFunc = void(*)(const nlohmann::json&, nlohmann::json&, std::ofstream&);

	NativeHostApp(OnTickFunc onMsg);
	~NativeHostApp();

	void Run();
	void Log(const char* msg);
private:
	std::ofstream m_Log;
	nlohmann::json m_InJSON, m_OutJSON;
	OnTickFunc m_OnMsg;

	static NativeHostApp* s_Instance;
private:
	bool ReadMsg();
	void SendMsg();
};

NativeHostApp* CreateNativeHostApp();