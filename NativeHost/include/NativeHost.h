#pragma once

#include "../src/vendor/glad/include/glad/glad.h"
#include "../src/NativeHostApp.h"
#include "../src/FunctionBinds.h"

int main()
{
	NativeHostApp* app = CreateNativeHostApp();
	app->Run();
	delete app;
	
	s_Logfile.close();

	return 0;
}