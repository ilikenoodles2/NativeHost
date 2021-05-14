#pragma once

#include <mutex>
#include <deque>
#include <sstream>

class Logger
{
public:
	static constexpr uint32_t OriginMask = 0x80000000u, RepeatMask = 0x70000000u, MaxLogRepeat = RepeatMask;

	Logger() {}
	~Logger() {}

	void Init(void* window);
	void Update(const float timestep);
	void Resize(int width, int height);
	void Shutdown();

	template<typename... Args>
	inline void Log(Args&&... args) { LogImpl(0, std::forward<Args>(args)...); }

	static constexpr int s_MsgCapacity = 100;
	static constexpr int s_HostMsgCapacity = 100;
private:
	mutable std::mutex m_Mutex;
	
	// If msb is 1, message is from the host,
	// if msb is 0, message is from the user
	std::deque<std::pair<std::string, uint32_t>> m_Buffer;
	int m_MsgCount = 0, m_HostMsgCount = 0;

	bool m_ShouldScrollToBottom = true;
private:
	template<typename... Args>
	void LogImpl(uint32_t origin, Args&&... args)
	{
		std::stringstream str;
		(str << ... << args);

		std::lock_guard<std::mutex> l(m_Mutex);
		if (m_Buffer.size() == 0)
		{
			m_Buffer.emplace_back(str.str(), origin);
			return;
		}

		auto& prevMsg = m_Buffer.back();
		if ((prevMsg.second & RepeatMask) == 0 && prevMsg.first == str.str())
		{
			if ((prevMsg.second & OriginMask) == origin)
			{
				prevMsg.first += "(%i)";
				prevMsg.second++;
				return;
			}
		}
		else if (prevMsg.first.compare(0, prevMsg.first.size() - 4, str.str()) == 0)
		{
			if ((prevMsg.second & OriginMask == origin)
				&& (prevMsg.second & RepeatMask) != MaxLogRepeat)
			{
				prevMsg.second++;
				return;
			}
		}

		if ((origin == 0 && m_MsgCount == s_MsgCapacity)
			|| (origin == OriginMask && m_HostMsgCount == s_HostMsgCapacity))
		{
			for (auto it = m_Buffer.begin(); it != m_Buffer.end(); it++)
			{
				if ((it->second & OriginMask) == origin)
				{
					m_Buffer.erase(it);
					m_Buffer.emplace_back(str.str(), origin);
					m_ShouldScrollToBottom = true;
					return;
				}
			}
		}

		m_Buffer.emplace_back(str.str(), origin);
		(origin == 0 ? m_MsgCount++ : m_HostMsgCount++);
		m_ShouldScrollToBottom = true;
	}

	template<typename... Args>
	inline void HostLog(Args&&... args) { LogImpl(OriginMask, std::forward<Args>(args)...); }

	friend class NativeHostApp;
};
