#pragma once

#include <ncurses.h>

#include <thread>
#include <mutex>
#include <vector>

namespace nctk {
#if 0
	class Input {
	public:
		Input();
		~Input();
		void start();
		void stop();
	private:
		std::thread * chReader;
		static void inputReader(Input* ic); // aka run
		std::mutex mtx; // keys mutex
		std::deque<char> keys;
	};
#endif
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

#if 0
class Window
{
public:
	Window( Window * parent );
private:
	std::vector<Window *>;
};
#endif
} // namespace nctk
