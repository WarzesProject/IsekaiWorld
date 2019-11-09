#pragma once

class Timer
{
public:
	Timer();
	void Reset();

	uint64_t GetMilliseconds() const;
	uint64_t GetMicroseconds() const;
	uint64_t GetStartMs() const;

private:
	std::chrono::high_resolution_clock m_HRClock;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
};