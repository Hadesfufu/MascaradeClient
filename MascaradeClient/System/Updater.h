
#ifndef UPDATER_H
#define UPDATER_H

#include <memory>
#include "DrawerStructures.h"
#include "Updatable.h"
#include "Singleton.h"
#include "Semaphore.h"
#include <list>



class Updater : public Singleton<Updater>
{
	friend class Singleton<Updater>;
public:

	void onUpdate();
	
	sf::Time getCurrentTime();

	void add(Updatable* ptr);
	void remove(Updatable* ptr);
	void clear();

private:

	struct ThreadData
	{
		std::vector<Updatable*>::iterator				begin;
		std::vector<Updatable*>::iterator				end;
		sf::Thread*		thread;
		sf::Mutex		StartingMutex;

		ThreadData::ThreadData()
		{
		}
	};

	Updater();
	~Updater();

private:

	void									t_func(ThreadData* TD);
	
	const bool								m_Threadified;
	const float								m_nbThreads;
	
	Semaphore								m_ThreadSemaphore;
	ThreadData*								m_ThreadData;

	sf::Clock								m_Clock;
	sf::Time								m_GameTime;
	std::vector<Updatable*>					m_Updatables;
	std::vector<Updatable*>					m_UpdatablesAddingBuffer;
	std::vector<Updatable*>					m_UpdatablesRemovingBuffer;
};

#endif

