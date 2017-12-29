////////////////////////////////////////////////////////////
// < Fichier : "Semaphore.h"> 
// < Author : Yohan "HadesFufu" Masson > 
//////////////////////////////////////////////////////////// 
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex>

class Semaphore
{
private:
	std::mutex mutex_;
	std::condition_variable condition_;
	unsigned long count_ = 0; // Initialized as locked.
	int currentlywaiting = 0;
public:

	void notifyall()
	{
		std::unique_lock<decltype(mutex_)> lock(mutex_);
		count_ += currentlywaiting;
		condition_.notify_all();
	}

	void notify() {
		std::unique_lock<decltype(mutex_)> lock(mutex_);
		++count_;
		condition_.notify_one();
	}

	void wait() {
		std::unique_lock<decltype(mutex_)> lock(mutex_);
		currentlywaiting++;
		while (!count_) // Handle spurious wake-ups.
			condition_.wait(lock);
		--count_;
		--currentlywaiting;
	}

	bool try_wait() {
		std::unique_lock<decltype(mutex_)> lock(mutex_);
		if (count_) {
			--count_;
			return true;
		}
		return false;
	}

	int getWaiting() { return currentlywaiting; }
};

#endif