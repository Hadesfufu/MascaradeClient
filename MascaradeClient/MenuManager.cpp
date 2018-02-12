#include "MenuManager.h"



MenuManager::MenuManager()
{
	NotificationManager::I()->AddObserver("menuManageClick", this, &MenuManager::manageClick);
	Input::Manager::I()->addMouseTrigger(sf::Event::EventType::MouseButtonReleased, sf::Mouse::Left, "menuManageClick");
	load();
}


MenuManager::~MenuManager()
{
}

void MenuManager::load()
{
	for(auto& elem : Data::I()->data.at("Menu"))
	{
		m_elements.push_back(MenuElement::create(elem));
	}
}

void MenuManager::manageClick(NotificationManager::Dictionary dic)
{
	Log::debug() << "lol mdr";
}
