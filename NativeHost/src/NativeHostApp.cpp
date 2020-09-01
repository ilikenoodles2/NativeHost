#include "NativeHostApp.h"

#include <string>
#include <iostream>
#include <string_view>

#ifdef _WIN32
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#endif

NativeHostApp* NativeHostApp::s_Instance = nullptr;

NativeHostApp::NativeHostApp(OnTickFunc onMsg)
	: m_OnMsg(onMsg)
{
	if (s_Instance) throw std::exception("Native Host already exists");
	s_Instance = this;

#ifdef _WIN32
	// On Windows, the default I / O mode is O_TEXT.Set this to O_BINARY
	// to avoid unwanted modifications of the input / output streams.
	if (_setmode(_fileno(stdin), _O_BINARY) == -1 || _setmode(_fileno(stdout), _O_BINARY) == -1)
	{
		throw std::exception("Failed to switch to binary mode");
	}
#endif
}

NativeHostApp::~NativeHostApp()
{
	m_Log.close();
}

void NativeHostApp::Run()
{
	while (ReadMsg())
		SendMsg();
}

void NativeHostApp::Log(const char* msg)
{
	m_Log << msg;
}

bool NativeHostApp::ReadMsg()
{
	static std::string buffer;

	// Read 4 bytes of info
	uint32_t msgLen;
	std::cin.read(reinterpret_cast<char*>(&msgLen), 4);
	if (msgLen == 0xcccccccc) return false;

	// read message
	buffer.resize(msgLen);
	std::cin.read(buffer.data(), msgLen);
	std::cin.clear();
	m_InJSON = nlohmann::json::parse(buffer);

	return true;
}

void NativeHostApp::SendMsg()
{
	m_OutJSON.clear();
	m_OnMsg(m_InJSON, m_OutJSON, m_Log);
	std::string str = m_OutJSON.dump();

	// Send 4 bytes of info
	uint32_t msgLen = str.size();
	std::cout.write(reinterpret_cast<char*>(&msgLen), 4);

	// Output message to chrome
	std::cout.write(str.c_str(), msgLen);
}