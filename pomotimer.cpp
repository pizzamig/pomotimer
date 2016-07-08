#include <time.h>
#include <signal.h>
#include <err.h>
#include <sysexits.h>

#include "pomotimer.h"

#include <iostream>

pomotimer::Config::Config(uint32_t focus, uint32_t shortBreak,
	 uint32_t longBreak, uint32_t loopSize) :
	 focus( focus ), shortBreak( shortBreak ),
	 longBreak( longBreak ), loopSize( loopSize )
{}

pomotimer::Config::Config() : focus( 25*60 ), shortBreak( 5*60 ),
  longBreak( 20*60 ), loopSize( 4 )
{}

int main()
{
  int rc;
  timer_t tid;
  rc = timer_create (CLOCK_MONOTONIC, nullptr, &tid );
  if ( rc == 0 ) {
    std::cout << "timer created - " << tid << std::endl;
  } else {
    err(EX_OSERR, nullptr);
  }
  
  return 0;
}

