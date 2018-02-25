#include "MenuManager.h"
#include "Drawer.h"


MenuManager::MenuManager() : m_rootContainer(nullptr)
{
	NotificationManager::I()->AddObserver("menuManageClick", this, &MenuManager::manageClick);
	Input::Manager::I()->addMouseTrigger(sf::Event::EventType::MouseButtonReleased, sf::Mouse::Left, "menuManageClick");
}


MenuManager::~MenuManager()
{
}

void MenuManager::load()
{
	m_rootContainer.load(Data::I()->data.at("Menu"));
	changeContainer(&m_rootContainer);
}

void MenuManager::manageClick(NotificationManager::Dictionary dic)
{
	Log::debug() << "lol mdr";
}

void MenuManager::changeContainer(MenuContainer* cont)
{
	m_currentContainer = cont;
	cont->putChildrenInDrawer();
}
