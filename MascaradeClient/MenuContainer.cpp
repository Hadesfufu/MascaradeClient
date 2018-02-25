#include "MenuContainer.h"
#include "Drawer.h"

MenuContainer::~MenuContainer()
{
}

void MenuContainer::load(nlohmann::json& j)
{
	if(Data::exist(j, "button"))
		m_button.load(j.at("button"));
	sf::Vector2f ruler = sf::Vector2f(0, Drawer::I()->getWindowSize().y);
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
	for(MenuElement* elem : m_elements)
	{
		Drawer::I()->addToLayer(0, elem);
	}
}

