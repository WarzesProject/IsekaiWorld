#include "stdafx.h"
#include "Error.h"

//-----------------------------------------------------------------------------
// ���������� ����
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
	TODO("����� ������ ������");
	CriticalErrorExit();
}
//-----------------------------------------------------------------------------