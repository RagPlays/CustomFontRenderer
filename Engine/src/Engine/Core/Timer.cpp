#include "enginepch.h"
#include "Timer.h"

#include <iostream>
#include <thread>

namespace Engine
{
	// Private Contructor
	Timer::Timer()
		: m_MillSecPerFrame{ 16 } //= 62.5 FPS
		, m_ElapsedSec{}
		, m_FpsCapped{}
		, m_LastTime{ std::chrono::high_resolution_clock::now() }
	{
	}

	Timer& Timer::Get()
	{
		static Timer timer{};
		return timer;
	}

	void Timer::Update()
	{
		// fps cap
		if (m_FpsCapped)
		{
			const auto sleeptime
			{
				m_LastTime + std::chrono::milliseconds(m_MillSecPerFrame) - std::chrono::high_resolution_clock::now()
			};

			if (sleeptime > std::chrono::nanoseconds::zero()) std::this_thread::sleep_for(sleeptime);
		}

		// rest
		const auto now{ std::chrono::high_resolution_clock::now() };
		m_ElapsedSec = std::chrono::duration<float>(now - m_LastTime).count();
		m_LastTime = now;

		/*if (m_ShowFPS)
		{
			m_AccumulatedTime += m_ElapsedSec;
			m_FrameCount++;
			if (m_AccumulatedTime >= 1.f)
			{
				std::cout << "FPS: " << static_cast<int>(m_FrameCount / m_AccumulatedTime) << "\n";
				m_AccumulatedTime -= 1.f;
				m_FrameCount = 0;
			}
		}*/
	}

	float Timer::GetSeconds() const
	{
		return m_ElapsedSec;
	}

	float Timer::GetMiliseconds() const
	{
		return m_ElapsedSec * 1000.f;
	}

	float Timer::GetFrameFPS() const
	{
		return 1.f / m_ElapsedSec;
	}

	void Timer::EnableFpsCap(int newFps)
	{
		if (newFps)
		{
			m_FpsCapped = true;
			m_MillSecPerFrame = static_cast<int>(1000.f / static_cast<float>(newFps));
		}
	}

	void Timer::DisableFpsCap()
	{
		m_FpsCapped = false;
	}
}