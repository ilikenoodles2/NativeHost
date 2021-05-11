#include "NativeHostApp.h"

#include <string>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#endif

NativeHostApp* NativeHostApp::s_Instance = nullptr;

NativeHostApp::NativeHostApp(Window::OnUpdate onUpdate, OnTickFunc onMsg)
	: m_Window(onUpdate), m_OnMsg(onMsg)
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

	bool windowInitialized = false;
	m_WindowProcess = std::thread([&]() {m_Window.StartProcess(m_Running, windowInitialized); });
	while (!windowInitialized) {}

	m_Window.SetContext(true);
}

NativeHostApp::~NativeHostApp()
{
	m_Window.~Window();
	m_WindowProcess.join();
}

void NativeHostApp::Run()
{
	m_Window.SetContext(false);
	m_Running = true;

	while (ReadMsg())
		SendMsg();
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

	// This is used instead of nlohmann::json:::parse(buffer);
	// to avoid redundant copy
	nlohmann::detail::parser<nlohmann::json>(nlohmann::detail::input_adapter(buffer), nullptr, true).parse(true, m_InJSON);
	GetLogger()->Log("Received: ", m_InJSON);

	return true;
}

void NativeHostApp::SendMsg()
{
	m_OutJSON.clear();
	m_OnMsg(m_InJSON, m_OutJSON);
	if (m_OutJSON.empty()) return;
	std::string str = m_OutJSON.dump();

	uint32_t msgLen = str.size();
	std::cout.write(reinterpret_cast<char*>(&msgLen), 4); // 4 bytes of info
	std::cout.write(str.c_str(), msgLen); // message

	GetLogger()->Log("Sending: ", str);
}