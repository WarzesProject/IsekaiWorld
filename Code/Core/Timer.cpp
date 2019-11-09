#include "stdafx.h"
#include "Timer.h"
//-----------------------------------------------------------------------------
using namespace std::chrono;
//-----------------------------------------------------------------------------
Timer::Timer()
{
	Reset();
}
//-----------------------------------------------------------------------------
void Timer::Reset()
{
	m_startTime = m_HRClock.now();
}
//-----------------------------------------------------------------------------
uint64_t Timer::GetMilliseconds() const
{
	auto newTime = m_HRClock.now();
	duration<double> dur = newTime - m_startTime;

	return duration_cast<milliseconds>(dur).count();
}
//-----------------------------------------------------------------------------
uint64_t Timer::GetMicroseconds() const
{
	auto newTime = m_HRClock.now();
	duration<double> dur = newTime - m_startTime;

	return duration_cast<microseconds>(dur).count();
}
//-----------------------------------------------------------------------------
uint64_t Timer::GetStartMs() const
{
	nanoseconds startTimeNs = m_startTime.time_since_epoch();
	return duration_cast<milliseconds>(startTimeNs).count();
}
//-----------------------------------------------------------------------------