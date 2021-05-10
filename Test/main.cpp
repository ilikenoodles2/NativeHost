#include "NativeHost.h"

struct Test
{
	int Add(int a, int b)
	{
		return a + b;
	}
};

int Add2(int a)
{
	return a + 2;
}

// OnUpdate is called within the window rendering loop
void OnUpdate(const float timestep)
{
}

// When chrome sends a message, the native host responds with a message
// (If there is no need to send a message to chrome, leave outJSON empty).
// For using FunctionBinds, inJSON is formatted as a JSON array with
// the first element as the name of the bound function, ex: ["Sub", 1, 1]
// calls Sub(1, 1), and outJSON becomes the return value of the function.
// Refer to https://github.com/nlohmann/json to deal with custom types.
void OnMsg(const nlohmann::json& inJSON, nlohmann::json& outJSON)
{
	FunctionBinds::Call(inJSON, outJSON);
}

// main() is implemented in NativeHost.h, all the user needs to do is
// do any necessesary setup before the native host runs, such as function
// binding.
NativeHostApp* CreateNativeHostApp()
{
	// Lambdas are not supported
	Test obj;
	FunctionBinds::Bind<&Test::Add>("Add", obj);
	FunctionBinds::Bind<&Add2>("Add2");

	return new NativeHostApp(OnUpdate, OnMsg);
}