
#include "TextureHolder.h"

////////////////////////////
// Constructor
////////////////////////////
TextureHolder::TextureHolder()
:m_Chemin_img("data/images/")
{

}

////////////////////////////
// Main Function
////////////////////////////
sf::Texture* TextureHolder::getTexture(const std::string& asName){
	// if the texture is already hold...
	if (m_Textures.find(asName) != m_Textures.end())
		return m_Textures.at(asName).get();
	// ... we return it. 
	sf::Texture* texture = new sf::Texture();
	// otherwise we load it
	if (texture->loadFromFile(m_Chemin_img + asName + ".png"))
	{
		// load success : we hold it and return a pointer
		m_Textures.emplace(asName, std::unique_ptr<sf::Texture>(texture));
		Log::info("Texture Chargée") << " ! path : " + m_Chemin_img + asName + ".png" ;
		return m_Textures.at(asName).get();
	}
	else
	{
		// load failed : we print a Error message and return a null pointer.
		delete texture; 
		Log::error("TextureHolder") << "Impossible de trouver la texture : path : " + m_Chemin_img + asName + ".png";
		return nullptr;
	}
}