#include "MenuElement.h"
#include "MenuButton.h"
#include "MenuContainer.h"


MenuElement::MenuElement()
{
}


MenuElement::~MenuElement()
{
}

MenuElement* MenuElement::create(json& j)
{
	if (j.at("type") == "button")
		return new MenuButton(j);
	else if (j.at("type") == "container")
		return new MenuContainer(j);
	else
		Log::error("MenuContainer::load") << "Element type not recognized";
}

