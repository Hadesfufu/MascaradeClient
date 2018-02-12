#include "MenuContainer.h"



MenuContainer::MenuContainer()
{
}


MenuContainer::~MenuContainer()
{
}

void MenuContainer::load(nlohmann::json& j)
{
	m_button.load(j.at("button"));
	for(auto& elem: j)
	{
		m_elements.push_back(MenuElement::create(elem));
	}
}

