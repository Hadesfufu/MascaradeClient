////////////////////////////////////////////////////////////
// < Fichier : "HUDSprite.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef HUDSprite_H
#define HUDSprite_H
#include <string>
#include "HUDElement.h"
namespace HUD {
	class Sprite : public Element
	{
	public:
		Sprite(Element *parent, json& node);
		~Sprite();

		void setSize(const sf::Vector2f& vec);
		void setPosition(const sf::Vector2f& vec);
		void setRotation(float f);
		void setScale(const sf::Vector2f& vec);
		void setOrigin(const sf::Vector2f& vec);

		virtual void updateFromCamera();
	private:
		void load(json& node);

	private:
		sf::Sprite	m_Sprite;
	};
}
#endif
