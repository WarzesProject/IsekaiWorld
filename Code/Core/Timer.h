#pragma once

class Timer
{
public:
	Timer();
	void Restart();
	double GetElapsedTime()const;
	float GetElapsedTimeF()const;

private:
	std::chrono::high_resolution_clock::time_point m_StartTime;
};