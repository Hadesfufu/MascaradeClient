
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
	Parameters *param = Parameters::I();
	float volume = (float)*param->getInt("MusicVolume") * (float)*param->getInt("GlobalVolume") / 100.0f;
	m_MusicDataAccess.lock();
	for (auto it = m_MusicPlayList.begin(); it != m_MusicPlayList.end(); ++it){
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2){
			it2->get()->setVolume(volume);
		}
	}
	m_MusicDataAccess.unlock();
}

void MusicManager::loadXML(){
	pugi::xml_document doc;
	if (!doc.load_file(Parameters::I()->getString("PlayListFile")->c_str())){
		Log::error("Parameters") << "Unable to open the parameters file";
		system("dir");
		return;
	}

	pugi::xml_node root = doc.first_child();
	pugi::xml_node elem = root.child("PlayList");
	pugi::xml_node track = elem.child("Track");
	std::string playlistname;
	while (elem){
		track = elem.child("Track");
		playlistname = elem.attribute("name").as_string();
		m_MusicPlayList.emplace(playlistname, MusicList());
		while (track){
			m_MusicPlayList[playlistname].push_back(std::make_shared<sf::Music>());
			m_MusicPlayList[playlistname].back()->openFromFile(track.attribute("path").as_string());
			Parameters *param = Parameters::I();
			float volume = (float)*param->getInt("MusicVolume") * (float)*param->getInt("GlobalVolume") / 100.0f;
			m_MusicPlayList[playlistname].back()->setVolume(volume);
			Log::info("MusicManager") << "Music Loaded for Playlist:" << playlistname << "- Song :" << track.attribute("path").as_string();
			track = track.next_sibling();
		}
		elem = elem.next_sibling();
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