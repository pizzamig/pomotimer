#pragma once

#include <cstdint>
#include <string>

namespace utility {
	/** Time to string converter.
	 * Utility class that given a number of seconds, return a string
	 * showing the time in 24h format 
	 */
	class Time {
	public:
		/** Constructor class.
		 * The constructor class takes one parameter, the number of seconds
		 * This parameter is limited ot maxTime; if the parameter exceeds this limit,
		 * it will be adjusted accordingly
		 * @param seconds The amount of seconds the time represent
		 */
		Time(uint32_t seconds);
		/** The converting function.
		 * This function converts the seconds in to a more human string
		 * The output format is HH:MM:SS
		 */
		std::string getTimeStr();
		/** The maximal amount of time.
		 * This static constant represent the maximal amount of seconds allowed to
		 * be converted in a string. The current limit is 86400, the amount of seconds
		 * representing a day (24h)
		 */
		static const uint32_t maxTime;
	private:
		uint32_t s;
	};

} // namespace utility
