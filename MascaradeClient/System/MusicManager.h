#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <memory>
#include "SFML\Audio.hpp"
#include "Singleton.h"
#include "Log.h"
#include "Parameters.h"
#include "pugixml.hpp"

typedef std::vector<std::shared_ptr<sf::Music>> MusicList;

class MusicManager : public Singleton<MusicManager>
{
	friend class Singleton<MusicManager>;
public:
	void changeVolume();

	void playList(std::string playListName);
	void play(std::string playListName, int track);

	void stop();
	void pause();
	void resume();

private:
	~MusicManager();
	MusicManager();

	void loadXML();
	
	void playNext();

	void t_Func();

	sf::Thread						 m_MusicThread;
	sf::Mutex						 m_MusicDataAccess;
	std::map<std::string, MusicList> m_MusicPlayList;

	sf::Music*						 m_CurrentPlayingMusic;
	sf::SoundSource::Status			 m_Status;

	bool							 m_Running;
};

//template Singleton<MusicManager>;

#endif