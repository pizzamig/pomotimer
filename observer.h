#pragma once
#include <vector>
#include <mutex>
#include <algorithm>

namespace utility {

/** A generic Observer abstract class.
 * This class is defined to be used in junction with Observable
 * It defines only one member that get a parameter as input
 * The Observervable call the notify() member when a event occur
 */
template< typename T >
class Observer
{
public:
	/** The abstract notify function.
	 * This function has to be implemented in the concrete Observer
	 * in order to be invoked by the Observable
	 */
	virtual void notify( T ) = 0;
};

/** A generic Observable class.
 * This class defines an Observable interface, providing callback invocations
 * the addObs() member register a new Observer
 * the notifyAllObs() notify all registere Observer
 * @todo Adding a de-registration function
 */
template< typename T >
class Observable {
	typedef Observer<T> Obs;
public:
	/** The observer registration method.
	 * A concrete Observer is registered in the internal structure.
	 * The function is thread-safe.
	 * @param o The pointer to a concrete Observer object
	 */
	void addObs( Obs * o )
	{
		if( o == nullptr )
			return;
		std::lock_guard<std::mutex> lock(mtx);
		observers.push_back( o );
	}
protected:
	/** The notification method.
	 * This function invokes hte notification callback of all registered Observers.
	 * If an Observer is not valid, it's ignored (double-check).
	 * The function is thread-safe.
	 * @param t The event parameter passed to the notification function
	 */
	void notifyAllObs(T t)
	{
		std::lock_guard<std::mutex> lock(mtx);
		std::for_each( observers.begin(), observers.end(),
					[t](Obs *o) { if( o != nullptr ) o->notify(t); } );
	}
	/** Removing all observer.
	 * This function clear the internal observers registry.
	 * All previously registered observer will be not notified anymore.
	 * The function is thread-safe.
	 */
	void removeAllObs()
	{
		std::lock_guard<std::mutex> lock(mtx);
		observers.clear();
	}

private:
	std::vector< Obs * > observers;
	std::mutex mtx;
};
} // namespace utility
