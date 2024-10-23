#ifndef TIMER_H
#define TIMER_H

#include <chrono>

#include "Singleton.h"

class Timer final : public Singleton<Timer>
{
public:

	virtual ~Timer() = default;

	Timer(const Timer& other) = delete;
	Timer(Timer&& other) noexcept = delete;
	Timer& operator=(const Timer& other) = delete;
	Timer& operator=(Timer&& other) noexcept = delete;

	// Update
	void Update();

	// Getters
	float GetElapsedSec() const;
	float GetFPS() const;

	// Setters
	void EnableFpsCap(int newFps);
	void DisableFpsCap();

private:

	friend class Singleton<Timer>;
	Timer();

private:

	int m_MillSecPerFrame;
	float m_ElapsedSec;
	bool m_FpsCapped;

	std::chrono::high_resolution_clock::time_point m_LastTime;

	bool m_ShowFPS;
	float m_AccumulatedTime;
	int m_FrameCount;

};

#endif // !TIMER_H
