#pragma once

#include "Subsystem.h"

class Timer;

class GlobalTime : public Subsystem<GlobalTime>
{
public:
	GlobalTime();
	~GlobalTime();

private:
	Timer *m_timer = nullptr;
};