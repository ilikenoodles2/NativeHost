#include "NativeHost.h"

void OnUpdate(const float timestep)
{
}

void OnMsg(const nlohmann::json& inJSON, nlohmann::json& outJSON)
{
}

NativeHostApp* CreateNativeHostApp()
{
	return new NativeHostApp(OnUpdate, OnMsg);
}