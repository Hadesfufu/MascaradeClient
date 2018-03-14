////////////////////////////////////////////////////////////
// < Fichier : "MenuElement.h"> 
// < Author : Yohan "HadesFufu" Masson > 
//////////////////////////////////////////////////////////// 
#ifndef MENUELEMENT_H
#define MENUELEMENT_H
#include "Data.h"
#include <SFML/Graphics/Drawable.hpp>
#include "InputManager.h"

class MenuContainer;

class MenuElement : public sf::Drawable
{
public:
	MenuElement(MenuContainer* parent) { m_parent = parent; }
	~MenuElement();

	virtual void load(json&) = 0;

	static MenuElement* create(MenuContainer* container,  json& j);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	virtual void setPosition(sf::Vector2f position) = 0;

protected:

	MenuContainer* m_parent;
private:

};


#endif