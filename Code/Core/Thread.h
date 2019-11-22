#pragma once

class Thread final
{
public:
	Thread() noexcept = default;

	Thread(const Thread&) = delete;
	Thread(Thread &&other) : m_thread(std::move(other.m_thread)) {}

	Thread(const std::thread&) = delete;
	Thread(std::thread &&other) noexcept : m_thread(std::move(other)) {}

	Thread& operator=(const Thread&) = delete;
	Thread& operator=(Thread &&other) noexcept
	{
		m_thread = std::move(other.m_thread);
		return *this;
	}

	Thread& operator=(const std::thread&) = delete;
	Thread& operator=(std::thread &&other) noexcept
	{
		m_thread = std::move(other);
		return *this;
	}

	template <typename Callable, typename... Args>
	explicit Thread(Callable &&f, Args&&... args) : m_thread(std::forward<Callable>(f), std::forward<Args>(args)...) {}

	~Thread()
	{
		if( m_thread.joinable() ) m_thread.join();
	}

	inline auto IsJoinable() const { return m_thread.joinable(); }
	inline auto GetId() const noexcept { return m_thread.get_id(); }
	inline auto GetNativeHandle() { return m_thread.native_handle(); }

	void Join()
	{
		m_thread.join();
	}

	inline void SetPriority(float priority, bool realtime)
	{
#if SE_COMPILER_MSVC
		static_cast<void>(realtime);
		static constexpr int priorities[] =
		{
			THREAD_PRIORITY_IDLE, THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL,
			THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST, THREAD_PRIORITY_TIME_CRITICAL
		};

		if( !::SetThreadPriority(m_thread.native_handle(), priorities[static_cast<size_t>((priority + 1.0F) * 6.0F / 2.0F)]) )
			throw std::system_error(GetLastError(), std::system_category(), "Failed to set thread name");
#else
		if( priority < 0.0F ) priority = 0.0F;

		const int policy = realtime ? SCHED_RR : SCHED_OTHER;

		const int minPriority = sched_get_priority_min(policy);
		if( minPriority == -1 )
			throw std::system_error(errno, std::system_category(), "Failed to get thread min priority");
		const int maxPriority = sched_get_priority_max(policy);
		if( maxPriority == -1 )
			throw std::system_error(errno, std::system_category(), "Failed to get thread max priority");

		sched_param param;
		param.sched_priority = static_cast<int>(priority * (maxPriority - minPriority)) + minPriority;
		const int error = pthread_setschedparam(t.native_handle(), policy, &param);
		if( error != 0 )
			throw std::system_error(error, std::system_category(), "Failed to set thread priority");

#endif
	}

	static inline void SetCurrentThreadName(const std::string &name)
	{
#if SE_COMPILER_MSVC
		constexpr DWORD MS_VC_EXCEPTION = 0x406D1388;
#  pragma pack(push,8)
		typedef struct tagTHREADNAME_INFO
		{
			DWORD dwType; // Must be 0x1000.
			LPCSTR szName; // Pointer to name (in user addr space).
			DWORD dwThreadID; // Thread ID (-1=caller thread).
			DWORD dwFlags; // Reserved for future use, must be zero.
		} THREADNAME_INFO;
#  pragma pack(pop)

		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = name.c_str();
		info.dwThreadID = static_cast<DWORD>(-1);
		info.dwFlags = 0;

		__try
		{
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR*>(&info));
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
		}
#else
#  if SE_PLATFORM_LINUX
		int error = pthread_setname_np(pthread_self(), name.c_str());
		if( error != 0 )
			throw std::system_error(error, std::system_category(), "Failed to set thread name");
#  endif
#endif
	}

private:
	std::thread m_thread;
};