////////////////////////////////////////////////////////////
// < Fichier : "HUDElement.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef HUDElement_H
#define HUDElement_H
#include <string>
#include "Drawer.h"
#include "NotificationManager.h"
#include "TextureHolder.h"
#include "FontHolder.h"

namespace HUD {
	class Element
	{
	public:
		Element(Element* parent, json& node);
		Element(Element* parent);
		~Element();

		virtual void load(json& node) = 0;

		virtual	void setSize(const sf::Vector2f& vec);
		virtual void setPosition(const sf::Vector2f& vec);
		/*
		 * Float overide position and set to left or bot
		 */

		virtual void setFloatRight();
		virtual void setFloatBot();
		virtual void setRotation(float f) = 0;
		virtual void setScale(const sf::Vector2f& vec) = 0;
		virtual void setOrigin(const sf::Vector2f& vec) { m_Origin = vec; };
		virtual void setOriginPercent(const sf::Vector2f& vec);
		virtual void setBounds(const sf::FloatRect& rect) { m_Bounds = rect; }

		void		 setCliping(bool b) { m_clip = b; }

		virtual void clip();
		///////////////////////////////////////////////////////
		// Percent Function are scaling relatively to the parent
		// It calculate new size and position
		///////////////////////////////////////////////////////

		virtual void setSizePercent(sf::Vector2f& vec);
		virtual void setPositionPercent(sf::Vector2f& vec);

		virtual void setPadding(sf::FloatRect& padding) { m_Padding = padding; };
		virtual void setMargin(sf::FloatRect& margin);

		virtual void updateFromGame(NotificationManager::Dictionary dic);
		virtual void updateFromCamera();

		sf::Vector2f	getSize() { return sf::Vector2f(m_Bounds.width, m_Bounds.height); }
		sf::Vector2f	getPosition() { return sf::Vector2f(m_Bounds.left, m_Bounds.top); }
		sf::FloatRect	getBounds() { return m_Bounds; }
		sf::Vector2f	getOrigin() { return m_Origin; }
		sf::FloatRect	getMargin() { return m_Margin; }
		sf::FloatRect	getPadding() { return m_Padding; }
		sf::FloatRect	getAvailableArea();
	protected:

		void loadElement(json& node);

	private:

		bool			checkParent();
	
	protected: 
		Element*		m_parent;
	private:

		sf::Vector2f	m_Origin;

		sf::FloatRect	m_Bounds;
		sf::FloatRect	m_Padding;
		sf::FloatRect	m_Margin;

		bool			m_clip;
	};
}
#endif
