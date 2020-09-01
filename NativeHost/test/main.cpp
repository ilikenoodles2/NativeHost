#include "NativeHost.h"

void OnMsg(const nlohmann::json& inJSON, nlohmann::json& outJSON, std::ofstream& log)
{
}

NativeHostApp* CreateNativeHostApp()
{
	return new NativeHostApp(OnMsg);
}