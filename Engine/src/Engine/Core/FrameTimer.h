#ifndef FRAMETIMER_H
#define FRAMETIMER_H

#include <chrono>

namespace Engine
{
	class FrameTimer  final
	{
	public:

		virtual ~FrameTimer() = default;

		FrameTimer(const FrameTimer& other) = delete;
		FrameTimer(FrameTimer&& other) noexcept = delete;
		FrameTimer& operator=(const FrameTimer& other) = delete;
		FrameTimer& operator=(FrameTimer&& other) noexcept = delete;

		// Get
		static FrameTimer& Get();

		// Update
		void Update();

		// Getters
		float GetSeconds() const;
		float GetMiliseconds() const;
		float GetFrameFPS() const;

	private:

		FrameTimer();

	private:

		float m_ElapsedSec;
		std::chrono::high_resolution_clock::time_point m_LastTime;

	};
}

#endif // !FRAMETIMER_H