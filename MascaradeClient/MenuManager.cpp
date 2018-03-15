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
	try {
		m_currentContainer = ((MenuButton*) dic.m_dict.at("button"))->getParentContainer();
		m_currentContainer->putChildrenInDrawer();
	}
	catch (...) {
		Log::error("MenuManager::changeContainer") << "badly made change container";
	}
}
