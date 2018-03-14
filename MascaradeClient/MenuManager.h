////////////////////////////////////////////////////////////
// < Fichier : "MenuManager.h"> 
// < Author : Yohan "HadesFufu" Masson > 
//////////////////////////////////////////////////////////// 
#ifndef MENUMANAGER_H
#define MENUMANAGER_H
#include "Singleton.h"
#include "MenuContainer.h"
#include "InputManager.h"

class MenuManager : public Singleton<MenuManager>
{
public:
	friend class Singleton<MenuManager>;

	MenuManager();
	~MenuManager();

	void load();

	void changeContainer(NotificationManager::Dictionary cont);

private:
	MenuContainer m_rootContainer;
	MenuContainer* m_currentContainer;
	//std::vector<MenuElement*> m_elements;
};


#endif