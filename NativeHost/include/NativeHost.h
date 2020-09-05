#pragma once

#include "../src/vendor/glad/include/glad/glad.h"
#include "../src/NativeHostApp.h"

int main()
{
	NativeHostApp* host = CreateNativeHostApp();
	host->Run();
	delete host;
	
	s_Logfile.close();

	return 0;
}