#include "MenuElement.h"
#include "MenuButton.h"
#include "MenuContainer.h"



MenuElement::~MenuElement()
{
}

MenuElement* MenuElement::create(MenuContainer* parent, json& j)
{
	if (j.at("type") == "button")
		return new MenuButton(parent, j);
	else if (j.at("type") == "container")
		return new MenuContainer(parent, j);
	else
		Log::error("MenuContainer::load") << "Element type not recognized";
}


void MenuElement::addToView() {
	Drawer::I()->addToLayer(0, this);
}