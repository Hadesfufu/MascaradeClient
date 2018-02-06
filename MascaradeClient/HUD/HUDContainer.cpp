#include "HUDContainer.h"
#include "HUDRectangle.h"
#include "HUDText.h"
#include "HUDSprite.h"

namespace HUD {

	Container::Container(Element* parent, json& node) : Element(parent, node)
	{
		load(node);
	}

	Container::~Container()
	{
		m_Elements.clear();
	}
	
	void Container::load(json& node)
	{
		m_Outline.setSize(getSize());
		m_Outline.setPosition(0, 0);
		m_Outline.setFillColor(sf::Color::Transparent);
		m_Outline.setOutlineColor(sf::Color::Red);
		m_Outline.setOutlineThickness(2.f);
		Drawer::I()->addToLayer("HUD", &m_Outline);
		setOutline(false);
		for (auto& child : node) {
			std::string childname = child.at("type");
			if (childname == "Container")
			{
				m_Elements.emplace_back(new Container(this, child));
			}
			else if (childname == "Rectangle")
			{
				m_Elements.emplace_back(new Rectangle(this, child));
			}
			else if (childname == "Text")
			{
				m_Elements.emplace_back(new Text(this, child));
			}
			else if (childname == "Sprite")
			{
				m_Elements.emplace_back(new Sprite(this, child));
			}
			else if (childname == "outline")
			{
				setOutline(true);
			}
		}
		Log::debug("HUDContainer") << "Container Loaded. Elements : " << m_Elements.size();
	}

	void Container::setPosition(const sf::Vector2f& vec)
	{
		Element::setPosition(vec);
	}
	
	void Container::setOrigin(const sf::Vector2f& vec)
	{
		Element::setOrigin(vec);
	}

	void Container::setRotation(float f)
	{
		//here set every element the origin of the container then rotate.
		//then restitute the origin back
		sf::Vector2f origin = getOrigin();
		sf::Vector2f basicPosElem;
		sf::Vector2f oldOrigin;
		for(auto it = m_Elements.begin(); it != m_Elements.end(); it++)
		{
			basicPosElem = (**it).getPosition() - (**it).getOrigin();
			oldOrigin = (**it).getOrigin();
			(**it).setOrigin(origin - basicPosElem);
			(**it).setRotation(f);
			(**it).setOrigin(oldOrigin);
		}
	}

	void Container::setScale(const sf::Vector2f& vec)
	{
		Log::debug("HUD::Container") << "Not implemented";
		return;
	}

	void Container::setSize(const sf::Vector2f& vec)
	{
		Element::setSize(vec);
	}

	void Container::updateFromCamera()
	{
		Element::updateFromCamera();
		m_Outline.setPosition(getPosition());
		for(auto it = m_Elements.begin(); it != m_Elements.end(); it++)
		{
			(**it).updateFromCamera();
		}
	}

}
