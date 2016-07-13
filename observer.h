#pragma once
#include <vector>
#include <mutex>
#include <algorithm>

namespace nctk {

template< typename T >
class Observer
{
public:
	virtual void notify( T ) = 0;
};

template< typename T >
class Observable {
	typedef Observer<T> Obs;
public:
	void addObs( Obs * o )
	{
		std::lock_guard<std::mutex> lock(mtx);
		observers.push_back( o );
	}
protected:
	void notifyAllObs(T t)
	{
		std::lock_guard<std::mutex> lock(mtx);
		std::for_each( observers.begin(), observers.end(),
					[t](Obs *o) { if( o != nullptr ) o->notify(t); } );
	}
private:
	std::vector< Obs * > observers;
	std::mutex mtx;
};
} // namespace nctk
