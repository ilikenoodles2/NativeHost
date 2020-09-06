#pragma once

#include <mutex>
#include <deque>
#include <sstream>

class Logger
{
public:
	Logger() {}
	~Logger() {}

	void Init(void* window);
	void Update(const float timestep);
	void Resize(int width, int height);
	void Shutdown();

	template<typename... Args>
	void Log(Args&&... args)
	{
		std::stringstream str;
		(str << ... << args);

		std::lock_guard<std::mutex> l(m_Mutex);
		if (m_Buffer.size() == s_MsgCapacity)
			m_Buffer.pop_front();

		m_Buffer.emplace_back(str.str());
		m_ShouldScrollToBottom = true;
	}

	static constexpr int s_MsgCapacity = 200;
private:
	mutable std::mutex m_Mutex;
	std::deque<std::string> m_Buffer;

	bool m_ShouldScrollToBottom = true;
};
