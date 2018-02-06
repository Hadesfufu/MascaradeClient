////////////////////////////////////////////////////////////
// < Fichier : "HUDText.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef HUDText_H
#define HUDText_H
#include <string>
#include "HUDElement.h"

namespace HUD {
	class Text : public Element
	{
	public:
		Text(Element* parent, json& node);
		~Text();

		void load(json& node);

		virtual void	setRotation(float f);
		virtual void	setScale(const sf::Vector2f& vec);
		void			setText(std::string s);

		virtual void	updateFromCamera();
		/*
		 * setViaNotif
		 * Dictionnary:
		 * [0] = string
		 * done
		 */
		void			setViaNotif(NotificationManager::Dictionary d);
	private:

	private:
		sf::Text m_text;
	};
}
#endif
