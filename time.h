#pragma once

#include <cstdint>
#include <string>

namespace utility {
	class Time {
	public:
		Time(uint32_t seconds);
		std::string getTimeStr();
	private:
		uint32_t s;
	};

} // namespace utility
