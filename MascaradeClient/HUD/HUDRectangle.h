////////////////////////////////////////////////////////////
// < Fichier : "HUDRectangle.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef HUDRectangle_H
#define HUDRectangle_H
#include <string>
#include "HUDElement.h"

namespace HUD {
	class Rectangle : public Element
	{
	public:
		Rectangle(Element *parent, json& node);
		~Rectangle();

		void setSize(const sf::Vector2f& vec);
		void setPosition(const sf::Vector2f& vec);
		void setRotation(float f);
		void setScale(const sf::Vector2f& vec);
		void setOrigin(const sf::Vector2f& vec);
		

		virtual void updateFromGame(NotificationManager::Dictionary dic);
		virtual void updateFromCamera();
	private:
		void load(json& node);

	private:
		sf::RectangleShape m_shape;
	};
}
#endif
