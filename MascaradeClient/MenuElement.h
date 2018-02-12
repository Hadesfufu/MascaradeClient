////////////////////////////////////////////////////////////
// < Fichier : "MenuElement.h"> 
// < Author : Yohan "HadesFufu" Masson > 
//////////////////////////////////////////////////////////// 
#ifndef MENUELEMENT_H
#define MENUELEMENT_H
#include "Data.h"
#include "Sprite3D.h"
#include <SFML/Graphics/Text.hpp>

class MenuElement
{
public:
	MenuElement();
	~MenuElement();

	virtual void load(json&) = 0;

	static MenuElement* create(json& j);
private:
	sf::Text		m_text;
	sf::Sprite3d	m_background;

};


#endif