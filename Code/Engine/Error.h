#pragma once

// Если произошла критическая ошибка
bool IsErrorCriticalExit();
void CriticalErrorExit();
void CriticalErrorExit(std::string_view str);