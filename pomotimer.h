#pragma once
#include <stdint.h>

namespace pomotimer {

class Config
{
public:
  Config();
  Config(uint32_t focus, uint32_t shortBreak,
		uint32_t longBreak, uint32_t loopSize );
	uint32_t getFocus() const { return focus; }
	uint32_t getShortBreak() const { return shortBreak; }
	uint32_t getLongBreak() const { return longBreak; }
	uint32_t getLoopSize() const { return loopSize; }
private:
  uint32_t focus;
  uint32_t shortBreak;
  uint32_t longBreak;
  uint32_t loopSize;
};

enum Timer { FOCUS, SHORT_BREAK, LONG_BREAK };
enum Status { RUN, STOP };

class Pomodoro {
public:
	Pomodoro( Config &c );
	uint32_t getTime() const { return time; }
	void update();
	Timer getTimerType() const { return type; }
private:
	Timer type;
	uint32_t time;
	uint32_t loopCounter; // short break counter
	Config & localConfig;
};

} // namespace
