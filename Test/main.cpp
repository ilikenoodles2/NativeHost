#include "NativeHost.h"

void OnMsg(const nlohmann::json& inJSON, nlohmann::json& outJSON)
{
}

NativeHostApp* CreateNativeHostApp()
{
	return new NativeHostApp(OnMsg);
}