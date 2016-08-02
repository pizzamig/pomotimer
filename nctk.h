#pragma once

#include <ncurses.h>

#include <thread>
#include <mutex>
#include <vector>
#include <exception>
#include <string>

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
enum class Errors : uint16_t { SIZE_TOO_SMALL };
class Exception : public std::exception
{
public:
	Exception(Errors e);
	virtual const char* what() const noexcept;
private:
	Errors e;
	std::string msg;
};

class Size
{
public:
	Size(int lines, int cols);
	int getCols() const { return cols; }
	int getLines() const { return lines; }
private:
	int lines;
	int cols;
};

class Application
{
public:
	Application();
	Application(const Size & minSize);
	~Application();
	int getRows() const { return rows; }
	int getCols() const { return cols; }
private:
	void init();
	int rows;
	int cols;
};

class Fatal
{
public:
	Fatal(Exception & e);
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

#include <ostream>

std::ostream&
operator<<(std::ostream& output, const nctk::Errors e);
