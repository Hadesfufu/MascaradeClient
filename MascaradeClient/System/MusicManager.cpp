
#include "MusicManager.h"

MusicManager::MusicManager() : 
m_CurrentPlayingMusic(nullptr),
m_Running(true),
m_MusicThread(&MusicManager::t_Func, this)
{
	//Log::info("Music Manager") << "Music Manager is disactivated due to opengl exception causef by thread";
	loadXML();
	m_MusicThread.launch();
}
MusicManager::~MusicManager()
{
	stop();
	m_Running = false;
	m_MusicThread.wait();
	m_MusicPlayList.clear();
}

void MusicManager::changeVolume(){
	float MusicVolume = Data::json().at("MusicVolume");
	float GlobalVolume = Data::json().at("GlobalVolume");
	float volume = MusicVolume * GlobalVolume / 100.0f;
	m_MusicDataAccess.lock();
	for (auto it = m_MusicPlayList.begin(); it != m_MusicPlayList.end(); ++it){
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2){
			it2->get()->setVolume(volume);
		}
	}
	m_MusicDataAccess.unlock();
}

void MusicManager::loadXML() {

	json playLists = Data::json().at("Playlists");
	std::string playlistname;
	std::string path;
	for (auto& playlist : playLists)
	{
		playlistname = playlist.at("name");
		m_MusicPlayList.emplace(playlistname, MusicList());
		for (auto& track : playlist.at("tracks"))
		{
			m_MusicPlayList[playlistname].push_back(std::make_shared<sf::Music>());
			path = track.at("path");
			m_MusicPlayList[playlistname].back()->openFromFile(path);
			Log::info("MusicManager") << "Music Loaded for Playlist:" << playlistname << "- Song :" << path;
		}
	}
}

void MusicManager::playList(std::string playListName){
	play(playListName, 0);
}

void MusicManager::play(std::string playListName, int track){
	if (m_MusicPlayList.find(playListName) == m_MusicPlayList.end()) {
		Log::error("Music Manager") << "No Playlist Found : " << playListName;
		return;
	}
	m_MusicDataAccess.lock();
	if (m_CurrentPlayingMusic)
		m_CurrentPlayingMusic->stop();

	m_MusicPlayList[playListName][track]->play();
	m_CurrentPlayingMusic = m_MusicPlayList[playListName][track].get();
	m_Status = m_CurrentPlayingMusic->getStatus();
	m_MusicDataAccess.unlock();
}
void MusicManager::stop(){
	if(!m_CurrentPlayingMusic)
	{
		Log::error("MusicManager") << "No Current music playing ";
		return;
	}
	m_MusicDataAccess.lock();
		m_CurrentPlayingMusic->stop();
		m_Status = m_CurrentPlayingMusic->getStatus();
	m_MusicDataAccess.unlock();
}
void MusicManager::resume(){
	if (!m_CurrentPlayingMusic)
	{
		Log::error("MusicManager") << "No Current music playing ";
		return;
	}
	m_MusicDataAccess.lock();
	if (m_Status == sf::SoundSource::Status::Paused && m_CurrentPlayingMusic){
		m_CurrentPlayingMusic->play();
		m_Status = m_CurrentPlayingMusic->getStatus();
	}
	m_MusicDataAccess.unlock();
}
void MusicManager::pause(){
	if (!m_CurrentPlayingMusic)
	{
		Log::error("MusicManager") << "No Current music playing ";
		return;
	}
	m_MusicDataAccess.lock();
		m_CurrentPlayingMusic->pause();
		m_Status = m_CurrentPlayingMusic->getStatus();
	m_MusicDataAccess.unlock();
}

void MusicManager::t_Func(){
	while (m_Running){
		m_MusicDataAccess.lock();
		if (m_CurrentPlayingMusic && 
			m_CurrentPlayingMusic->getStatus() == sf::SoundSource::Status::Stopped 
			&& m_Status != sf::SoundSource::Status::Stopped)
				playNext();
		m_MusicDataAccess.unlock();
	}
}

void MusicManager::playNext(){
	bool found = false;
	sf::Music* nextsong = nullptr;
	for (auto it = m_MusicPlayList.begin(); it != m_MusicPlayList.end() && !found; ++it)
    {
		for (auto it2 = it->second.begin(); it2 != it->second.end() && !found; ++it2)
		{
			if (m_CurrentPlayingMusic == it2->get()){
				found = true;
				++it2;
				
				if (it2 == it->second.end())
					it2 = it->second.begin();
				
				nextsong = it2->get();
			}
		}
    }
	if (nextsong){
		m_MusicDataAccess.lock();
		m_CurrentPlayingMusic = nextsong;
		m_CurrentPlayingMusic->play();
		m_Status = m_CurrentPlayingMusic->getStatus();
		m_MusicDataAccess.unlock();
	}

}