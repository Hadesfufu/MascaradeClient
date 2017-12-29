
#include "Updater.h"
#include <thread>
#include <iostream>
#include "Parameters.h"

Updater::Updater() : m_nbThreads(std::thread::hardware_concurrency()-1), m_Threadified(*Parameters::I()->getBool("UpdateThread"))
{
	if(m_Threadified){
		m_ThreadData =	new ThreadData[m_nbThreads];
		for (int i = 0; i < m_nbThreads; i++)
		{
			auto fn = &Updater::t_func;
			m_ThreadData[i].thread = new sf::Thread(std::bind(fn, this, &m_ThreadData[i]), this);
			m_ThreadData[i].StartingMutex.lock();
			m_ThreadData[i].thread->launch();
		}
	}
}


Updater::~Updater()
{
	if (m_Threadified) {
		for (int i = 0; i < m_nbThreads; i++)
		{
			m_ThreadData[i].thread->terminate();
			delete m_ThreadData[i].thread;
		}
		delete[] m_ThreadData;
	}
}

void Updater::onUpdate()
{

	m_GameTime = m_Clock.getElapsedTime();
	m_Clock.restart();

	for (auto it = m_UpdatablesAddingBuffer.begin(); it != m_UpdatablesAddingBuffer.end(); ++it)
	{
		m_Updatables.push_back(*it);
	}
	m_UpdatablesAddingBuffer.clear();

	for (auto it = m_UpdatablesRemovingBuffer.begin(); it != m_UpdatablesRemovingBuffer.end(); ++it)
	{
		auto deletor = std::find(m_Updatables.begin(), m_Updatables.end(), *it);
		m_Updatables.erase(deletor);
	}
	m_UpdatablesRemovingBuffer.clear();

	if (m_Threadified) {

		float step = m_Updatables.size() / m_nbThreads;
		
		for (int i = 0; i < m_nbThreads; i++)
		{
			m_ThreadData[i].begin = m_Updatables.begin() + (i*step) ;
			m_ThreadData[i].end = m_Updatables.begin() + (i + 1)*step;
			m_ThreadData[i].StartingMutex.unlock();
		}
		m_ThreadSemaphore.notifyall();

		while (m_ThreadSemaphore.getWaiting() < m_nbThreads) {}
	}
	else {
		for (auto it = m_Updatables.begin(); it != m_Updatables.end(); ++it)
		{
			if (*it)
				(*it)->onUpdate(m_GameTime);
			else
				it = m_Updatables.erase(it);
		}
	}
}

sf::Time Updater::getCurrentTime()
{
	return m_GameTime;
}


void Updater::add(Updatable* ptr) {
	if(ptr)
		m_UpdatablesAddingBuffer.push_back(ptr);
}

void Updater::remove(Updatable* ptr)
{
	if(ptr)
		m_UpdatablesRemovingBuffer.push_back(ptr);
}

void Updater::clear()
{
	m_Updatables.clear();
	m_UpdatablesAddingBuffer.clear();
	m_UpdatablesRemovingBuffer.clear();
}


void Updater::t_func(ThreadData* TD)
{
	while (true)
	{
		if(!TD)
		{
			Log::error("Updater - Thread") << "Thread is corrupted :";
			return;
		}
		
		TD->StartingMutex.lock();
		//m_ThreadSemaphore.wait();
		for (auto it = TD->begin; it != TD->end; ++it)
		{
			if (*it)
				(*it)->onUpdate(m_GameTime);
			else
				m_UpdatablesRemovingBuffer.push_back(*it);
		}

	}
}
