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
	MenuContainer();
	MenuContainer(json& j) { load(j); };
	~MenuContainer();


	virtual void load(nlohmann::json&);

private:
	MenuButton				 m_button;
	std::vector<MenuElement*> m_elements;
};


#endif