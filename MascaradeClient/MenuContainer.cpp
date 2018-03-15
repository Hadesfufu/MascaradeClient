#include "MenuContainer.h"
#include "Drawer.h"
#include "MenuManager.h"

MenuContainer::~MenuContainer()
{
}

void MenuContainer::load(nlohmann::json& j)
{
	if(Data::exist(j, "button"))
		m_button.load(j.at("button"));
	m_button.setAction("changeContainer");
	sf::Vector2f ruler = sf::Vector2f(0, Drawer::I()->getWindowSize().y);
	
	if (!Data::exist(j, "elements"))
		return;

	float nbItem = j.at("elements").size();
	float i = 1;
	float multiple;
	for(auto& elem: j.at("elements"))
	{
		MenuElement* newElement = MenuElement::create(this, elem);
		multiple = (i / (nbItem + 1));
		newElement->setPosition(ruler * multiple);
		m_elements.push_back(newElement);
		i++;
	}
}

void MenuContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_button);
}

void MenuContainer::putChildrenInDrawer()
{
	Drawer::I()->removeLayer(0);
	Input::Manager::I()->removeMouseTrigger(sf::Event::MouseButtonPressed, sf::Mouse::Button::Left);
	Log::debug() << this;
	for (auto it = m_elements.begin(); it != m_elements.end(); ++it) {
		(*it)->addToView(); 
	}
}