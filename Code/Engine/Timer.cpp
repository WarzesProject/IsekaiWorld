#include "stdafx.h"
#include "Timer.h"
//-----------------------------------------------------------------------------
using namespace std::chrono;
//-----------------------------------------------------------------------------
template<typename T>
T getElapsedTime(high_resolution_clock::time_point StartTime)
{
	auto CurrTime = high_resolution_clock::now();
	auto time_span = duration_cast<duration<T>>(CurrTime - StartTime);
	return time_span.count();
}
//-----------------------------------------------------------------------------
Timer::Timer()
{
	Restart();
}
//-----------------------------------------------------------------------------
void Timer::Restart()
{
	m_StartTime = high_resolution_clock().now();
}
//-----------------------------------------------------------------------------
double Timer::GetElapsedTime()const
{
	return getElapsedTime<double>(m_StartTime);
}
//-----------------------------------------------------------------------------
float Timer::GetElapsedTimeF()const
{

	return getElapsedTime<float>(m_StartTime);
}
//-----------------------------------------------------------------------------