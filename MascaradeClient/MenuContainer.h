////////////////////////////////////////////////////////////
// < Fichier : "MenuContainer.h"> 
// < Author : Yohan "HadesFufu" Masson > 
//////////////////////////////////////////////////////////// 
#ifndef MENUCONTAINER_H
#define MENUCONTAINER_H
#include "MenuElement.h"
#include "MenuButton.h"

class MenuContainer : public MenuElement
{
public:
	MenuContainer(MenuContainer* parent) : MenuElement(parent), m_button(this) {}
	MenuContainer(MenuContainer* parent, json& j) : MenuElement(parent), m_button(this) { load(j); };
	~MenuContainer();



	virtual void load(nlohmann::json&);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual	void setPosition(sf::Vector2f position) { m_button.setPosition(position); };
	virtual void setMyselfAsCurrentContainer();
	void putChildrenInDrawer();

private:
	MenuContainer*			 m_parent;
	MenuButton				 m_button;
	std::vector<MenuElement*> m_elements;
};


#endif