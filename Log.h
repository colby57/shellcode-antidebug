#pragma once

#define log(format, ...) \
		logger::Log(format, __VA_ARGS__);

namespace logger
{
	template <typename ... T>

	__forceinline void Log(const char *format, T const& ... args)
	{
		std::printf(format, args ...);
	}
}
