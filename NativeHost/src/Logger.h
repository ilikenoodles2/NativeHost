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
				if (prevMsg.second != 0x4000)
				{
					prevMsg.second++;
					return;
				}
		}
		else if (prevMsg.first.compare(0, prevMsg.first.size() - 4, str.str()) == 0)
		{
			if (prevMsg.second != 0x40000000)
			{
				prevMsg.second++;
				return;
			}
		}

		if (m_Buffer.size() == s_MsgCapacity)
			m_Buffer.pop_front();

		m_Buffer.emplace_back(str.str(), 0);
		m_ShouldScrollToBottom = true;
	}

	static constexpr int s_MsgCapacity = 200;
private:
	mutable std::mutex m_Mutex;
	
	// If msb is 1, message is from the host,
	// if msb is 0, message is from the user
	std::deque<std::pair<std::string, uint32_t>> m_Buffer;

	bool m_ShouldScrollToBottom = true;
private:
	template<typename... Args>
	void HostLog(Args&&... args)
	{
		Log(std::forward<Args>(args)...);
		m_Buffer.back().second |= 0x80000000;
	}

	friend class NativeHostApp;
};
