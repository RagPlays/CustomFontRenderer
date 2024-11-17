#include "enginepch.h"
#include "FrameTimer.h"

namespace Engine
{
	// Private Contructor
	FrameTimer::FrameTimer()
		: m_ElapsedSec{}
		, m_LastTime{ std::chrono::high_resolution_clock::now() }
	{
	}

	FrameTimer& FrameTimer::Get()
	{
		static FrameTimer timer{};
		return timer;
	}

	void FrameTimer::Update()
	{
		const auto now{ std::chrono::high_resolution_clock::now() };
		m_ElapsedSec = std::chrono::duration<float>(now - m_LastTime).count();
		m_LastTime = now;
	}

	float FrameTimer::GetSeconds() const
	{
		return m_ElapsedSec;
	}

	float FrameTimer::GetMiliseconds() const
	{
		return m_ElapsedSec * 1000.f;
	}

	float FrameTimer::GetFrameFPS() const
	{
		return 1.f / m_ElapsedSec;
	}
}