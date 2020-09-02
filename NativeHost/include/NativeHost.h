#pragma once

#include "../src/NativeHostApp.h"

int main()
{
	NativeHostApp* host = CreateNativeHostApp();
	host->Run();
	delete host;
	return 0;
}