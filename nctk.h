#pragma once

#include <ncurses.h>

namespace nctk {
	
class Application
{
public:
	Application();
	~Application();
	int getRows() const { return rows; }
	int getCols() const { return cols; }
private:
	int rows;
	int cols;
};

} // namespace nctk
