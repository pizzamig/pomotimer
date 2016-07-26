#include "time.h"
#include <sstream>

utility::Time::Time(uint32_t seconds) :
	s(seconds)
{}

std::string
utility::Time::getTimeStr()
{
	uint32_t sec,min,h;
	uint32_t tt=s;
	sec=tt%60;
	tt/=60;
	min=tt%60;
	tt/=60;
	h=tt;
	std::ostringstream str;
	str << h << ":" << min << ":" << sec;
	return str.str();
}
