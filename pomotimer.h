#pragma once
#include <stdint.h>

namespace pomotimer {

class Config
{
public:
  Config();
  Config(uint32_t focus, uint32_t shortBreak,
	 uint32_t longBreak, uint32_t loopSize );
private:
  uint32_t focus;
  uint32_t shortBreak;
  uint32_t longBreak;
  uint32_t loopSize;
};

enum Timer { FOCUS, SHORT_BREAK, LONG_BREAK };

class Pomodoro {
public:
	Pomodoro();
	uint32_t getTime() const { return time; }
	void update();
	Timer getTimerType() const { return type; }
private:
	Timer type;
	uint32_t time;
	uint32_t loopCounter; // short break counter
};

enum Status { RUN, STOP };

} // namespace
