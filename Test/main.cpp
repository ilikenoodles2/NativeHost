#include "NativeHost.h"

struct Test
{
	int add(int a, int b)
	{
		return a + b;
	}
};

int sub(int a, int b)
{
	return a - b;
}

void OnUpdate(const float timestep)
{
}

void OnMsg(const nlohmann::json& inJSON, nlohmann::json& outJSON)
{
	FunctionBinds::Call(inJSON, outJSON);
}

NativeHostApp* CreateNativeHostApp()
{
	Test t;
	FunctionBinds::Bind<&Test::add>("Add", t);
	FunctionBinds::Bind<&sub>("Sub");

	return new NativeHostApp(OnUpdate, OnMsg);
}