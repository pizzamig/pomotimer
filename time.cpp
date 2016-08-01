#include "time.h"
#include <cstdio>

uint32_t utility::Time::maxTime=(24*60*60)-1;

utility::Time::Time(uint32_t seconds) :
	s(seconds)
{
	if( seconds > maxTime ) {
		s = maxTime;
	}
}

std::string
utility::Time::getTimeStr()
{
	char strBuf[9];
	uint32_t ss,mm,hh;
	uint32_t tt=s;
	ss=tt%60;
	tt/=60;
	mm=tt%60;
	hh=tt/60;
	std::snprintf(strBuf,9,"%02u:%02u:%02u", hh,mm,ss);
	return std::string(strBuf);
}
