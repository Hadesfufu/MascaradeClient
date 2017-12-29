#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H
////////////////////////////////////////////////////////////
//
//-------------------BY HadesFufu---------------------
// >French<
//Utilisation : Cette classe sert de stockage aux differentes textures
//afin de ne les charger qu'une seule fois dans la mémoire.
//A chaque recupération de texture externe -> passé par cette classe.
//
//Fonctionnement : Prend le chemin de l'image. la stocker si elle existe 
//et qu'elle n'est pas déja dans la mémoire. Retourne un pointeur sur 
//la Texture.
//
////////////////////////////////////////////////////////////

////////////////////////////
// INCLUDED HEADERS
////////////////////////////
#include <SFML/Graphics.hpp>
#include <memory>
#include "Log.h"
#include "Singleton.h"

////////////////////////////
// TEXTURE HOLDER
////////////////////////////
class TextureHolder : public Singleton<TextureHolder>
{
	friend class Singleton<TextureHolder>;
public:
	
	////////////////////////////
	// Main Function
	// @effect:		if the texture is arleady loaded, it returns a pointer on it,
	// @effect:		else it load the texture and stock it into : m_textures.
	// @param:	(const std::string)		name of the Texture.
	// @return:		(sf::Texture*)			pointer on the Texture. 
	////////////////////////////
	sf::Texture* getTexture(const std::string&);
private:

	////////////////////////////
	// Constructor
	////////////////////////////
	TextureHolder();
	TextureHolder(const TextureHolder&) = delete;

	////////////////////////////
	// Destructor
	////////////////////////////
	~TextureHolder(){ m_Textures.clear(); };

	////////////////////////////
	// sf::Texture Storage
	////////////////////////////
	std::map<std::string, std::unique_ptr<sf::Texture>> m_Textures;

	////////////////////////////
	// Constant path for textures.
	////////////////////////////
	const std::string			m_Chemin_img;
};

#endif
