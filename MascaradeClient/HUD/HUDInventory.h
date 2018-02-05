////////////////////////////////////////////////////////////
// < Fichier : "HUDInventory.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef HUDInventory_H
#define HUDInventory_H
#include "HUDElement.h"
#include "HUDSprite.h"

namespace HUD {

	class Inventory : public Element
	{

	public:
		Inventory(Element* parent, pugi::xml_node& node);
		~Inventory();

		virtual void load(pugi::xml_node& node) = 0;

		virtual void setRotation(float f) = 0;
		virtual void setScale(const sf::Vector2f& vec) = 0;
	
	private:

	};
}
#endif
