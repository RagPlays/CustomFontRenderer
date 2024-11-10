#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace Engine
{
	class Timer final
	{
	public:

		virtual ~Timer() = default;

		Timer(const Timer& other) = delete;
		Timer(Timer&& other) noexcept = delete;
		Timer& operator=(const Timer& other) = delete;
		Timer& operator=(Timer&& other) noexcept = delete;

		// Get
		static Timer& Get();

		// Update
		void Update();

		// Getters
		float GetSeconds() const;
		float GetMiliseconds() const;
		float GetFrameFPS() const;

		// Setters
		void EnableFpsCap(int newFps);
		void DisableFpsCap();

	private:

		Timer();

	private:

		int m_MillSecPerFrame;
		float m_ElapsedSec;
		bool m_FpsCapped;

		std::chrono::high_resolution_clock::time_point m_LastTime;

	};
}

#endif // !TIMER_H
