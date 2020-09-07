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
		if (m_Buffer.size() == 0)
		{
			m_Buffer.emplace_back(str.str(), 0);
			return;
		}

		auto& prevMsg = m_Buffer.back();
		if (m_Buffer.back().second == 0 && prevMsg.first == str.str())
		{
				prevMsg.first += "(%i)";
				prevMsg.second++;
		}
		else if (prevMsg.first.compare(0, prevMsg.first.size() - 4, str.str()) == 0)
			prevMsg.second++;
		else
		{
			if (m_Buffer.size() == s_MsgCapacity)
				m_Buffer.pop_front();

			m_Buffer.emplace_back(str.str(), 0);
		}

		m_ShouldScrollToBottom = true;
	}

	static constexpr int s_MsgCapacity = 200;
private:
	mutable std::mutex m_Mutex;
	std::deque<std::pair<std::string, int>> m_Buffer;

	bool m_ShouldScrollToBottom = true;
};
