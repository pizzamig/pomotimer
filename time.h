#pragma once

#include <cstdint>
#include <string>

namespace utility {
	class Time {
	public:
		Time(uint32_t seconds);
		std::string getTimeStr();
		static uint32_t maxTime;
	private:
		uint32_t s;
	};

} // namespace utility
