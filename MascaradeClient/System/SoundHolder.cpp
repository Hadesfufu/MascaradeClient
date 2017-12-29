
#include "SoundHolder.h"

////////////////////////////
// Constructor
////////////////////////////
SoundHolder::SoundHolder()
:m_path_sound("data/sounds/")
{

}

////////////////////////////
// Main Function
////////////////////////////
sf::SoundBuffer* SoundHolder::getSound(const std::string& asName){
	// if the texture is already hold...
	if (m_Sounds.find(asName) != m_Sounds.end())
		return m_Sounds.at(asName).get();
	// ... we return it. 
	sf::SoundBuffer* sound = new sf::SoundBuffer();
	// otherwise we load it
	if (sound->loadFromFile(m_path_sound + asName + ".png"))
	{
		// load success : we hold it and return a pointer
		m_Sounds.emplace(asName, std::unique_ptr<sf::SoundBuffer>(sound));
		Log::info("Texture Chargée") << " ! path : " + m_path_sound + asName + ".png";
		return sound;
	}
	else
	{
		// load failed : we print a Error message and return a null pointer.
		delete sound;
		Log::error("TextureHolder") << "Impossible de trouver la texture : path : " + m_path_sound + asName + ".png";
		return nullptr;
	}
}