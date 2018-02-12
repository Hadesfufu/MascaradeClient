////////////////////////////////////////////////////////////
// < Fichier : "MenuButton.h"> 
// < Author : Yohan "HadesFufu" Masson > 
//////////////////////////////////////////////////////////// 
#ifndef MENUBUTTON_H
#define MENUBUTTON_H
#include "MenuElement.h"
#include "Sprite3D.h"

class MenuButton : public MenuElement
{
public:
	MenuButton();
	MenuButton(json& j) { load(j); };
	~MenuButton();


	virtual void load(nlohmann::json&);

private:
	sf::Sprite3d m_sprite;

};

#endif