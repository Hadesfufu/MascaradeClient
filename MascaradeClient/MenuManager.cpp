#include "MenuManager.h"
#include "Drawer.h"


MenuManager::MenuManager() : m_rootContainer(nullptr)
{
	NotificationManager::I()->AddObserver("changeContainer", this, &MenuManager::changeContainer);
}


MenuManager::~MenuManager()
{
}

void MenuManager::load()
{
	m_rootContainer.load(Data::I()->data.at("Menu"));
	m_currentContainer = &m_rootContainer;
	m_currentContainer->putChildrenInDrawer();
}

void MenuManager::changeContainer(NotificationManager::Dictionary dic)
{
	if (dic.m_object) {
		Log::error("MenuManager::changeContainer") << "badly made change container";
	}
	m_currentContainer = (MenuContainer*) dic.m_object;
	m_currentContainer->putChildrenInDrawer();
}
