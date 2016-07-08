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

} // namespace