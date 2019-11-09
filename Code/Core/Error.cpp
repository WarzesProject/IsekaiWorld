#include "stdafx.h"
#include "Error.h"
#include "Log.h"
//-----------------------------------------------------------------------------
// דכמבאכüםûי פכאד
extern bool Error_IsErrorCriticalExit = false;
//-----------------------------------------------------------------------------
bool IsErrorCriticalExit()
{
	return Error_IsErrorCriticalExit;
}
//-----------------------------------------------------------------------------
void CriticalErrorExit()
{
	Error_IsErrorCriticalExit = true;
}
//-----------------------------------------------------------------------------
void CriticalErrorExit(std::string_view str)
{
	Log::Error(str);
	CriticalErrorExit();
}
//-----------------------------------------------------------------------------