////////////////////////////////////////////////////////////
// < Fichier : "HUDContainer.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef HUDContainer_H
#define HUDContainer_H
#include <string>
#include "zeraTeam_system.h"
#include "HUDElement.h"
namespace HUD {
	class Container : public Element
	{
	public:
		Container(Element *parent, pugi::xml_node& node);
		~Container();

		void setSize(const sf::Vector2f& vec);
		void setPosition(const sf::Vector2f& vec);
		void setRotation(float f);
		void setScale(const sf::Vector2f& vec);
		void setOrigin(const sf::Vector2f& vec);
		void setOutline(bool b) { if (b) Drawer::I()->showElement(&m_Outline); else Drawer::I()->hideElement(&m_Outline); }

		void updateFromCamera();
	private:
		void load(pugi::xml_node& node);

	private:
		std::vector<std::shared_ptr<Element>> m_Elements;
		sf::RectangleShape						m_Outline;
	};
}
#endif
