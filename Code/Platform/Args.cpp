#include "stdafx.h"
#include "Args.h"
//-----------------------------------------------------------------------------
struct ArgsPimpl
{
	int argc = 0;
#if SE_COMPILER_MSVC
	LPWSTR *argv = nullptr;
#endif
};
//-----------------------------------------------------------------------------
Args::Args()
	: m_impl(new ArgsPimpl)
{
#if SE_COMPILER_MSVC
	m_impl->argv = CommandLineToArgvW(GetCommandLineW(), &m_impl->argc);

	if (m_impl->argv)
	{
		int bufferSize;
		std::vector<char> buffer;

		for (int i = 0; i < m_impl->argc; ++i)
		{
			bufferSize = WideCharToMultiByte(CP_UTF8, 0, m_impl->argv[i], -1, nullptr, 0, nullptr, nullptr);
			if (bufferSize == 0)
			{
				CriticalErrorExit("Failed to convert wide char to UTF-8");
				return;
			}				

			buffer.resize(bufferSize);
			if (WideCharToMultiByte(CP_UTF8, 0, m_impl->argv[i], -1, buffer.data(), bufferSize, nullptr, nullptr) == 0)
			{
				CriticalErrorExit("Failed to convert wide char to UTF-8");
				return;
			}

			m_args.push_back(buffer.data());
		}
	}
#endif
	setValid(true);
}
//-----------------------------------------------------------------------------
Args::~Args()
{
#if SE_COMPILER_MSVC
	if (m_impl->argv) LocalFree(m_impl->argv);
#endif
	SafeDelete(m_impl);
}
//-----------------------------------------------------------------------------