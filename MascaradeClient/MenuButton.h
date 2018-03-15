////////////////////////////////////////////////////////////
// < Fichier : "MenuButton.h"> 
// < Author : Yohan "HadesFufu" Masson > 
//////////////////////////////////////////////////////////// 
#ifndef MENUBUTTON_H
#define MENUBUTTON_H
#include "MenuElement.h"
#include "Sprite3D.h"
#include <SFML/Graphics/Text.hpp>
#include "TextureHolder.h"

class MenuButton : public MenuElement
{
public:
	MenuButton(MenuContainer* parent) : MenuElement(parent){}
	MenuButton(MenuContainer* parent, json& j) : MenuElement(parent) { load(j); };
	~MenuButton();


	virtual void load(nlohmann::json&);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void addToView();

	virtual void setPosition(sf::Vector2f position);
	virtual void setAction(std::string s) { m_action = s; }
	virtual void registerAction();

private:
	sf::Text		m_text;
	sf::Sprite3d	m_background;
	std::string		m_action;
};

#endif