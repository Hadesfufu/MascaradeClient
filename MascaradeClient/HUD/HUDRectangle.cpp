#include "HUDRectangle.h"

namespace HUD
{
	Rectangle::Rectangle(Element* parent, pugi::xml_node& node) : Element(parent, node)
	{
		load(node);
	}

	Rectangle::~Rectangle()
	{
		
	}
	
	void Rectangle::load(pugi::xml_node& node)
	{
		sf::Color color;
		pugi::xml_node childLoader;
		std::string s;
		float f;
		
		/////////////////
		//FillColor
		/////////////////
		if (childLoader = node.child("fillColor")) {
			color.r = childLoader.attribute("r").as_int();
			color.g = childLoader.attribute("g").as_int();
			color.b = childLoader.attribute("b").as_int();
			color.a = childLoader.attribute("a").as_int();
			m_shape.setFillColor(color);
		}

		/////////////////
		//OutlineColor
		/////////////////
		if (childLoader = node.child("outlineColor")) {
			color.r = childLoader.attribute("r").as_int();
			color.g = childLoader.attribute("g").as_int();
			color.b = childLoader.attribute("b").as_int();
			color.a = childLoader.attribute("a").as_int();
			m_shape.setOutlineColor(color);
		}

		/////////////////
		//OutlineThickness
		/////////////////
		if (childLoader = node.child("outlineThickness")) {
			f = childLoader.first_attribute().as_float();
			m_shape.setOutlineThickness(f);
		}

		/////////////////
		//Action
		/////////////////
		if(childLoader = node.child("action"))
		{
			s = childLoader.first_attribute().as_string();
			NotificationManager::I()->AddObserver(s, this, &Rectangle::updateFromGame);			
		}
		m_shape.setSize(getSize());
		m_shape.setOrigin(getOrigin());
		m_shape.setPosition(getPosition());
		Drawer::I()->addToLayer("HUD", &m_shape);
	}

	void Rectangle::setPosition(const sf::Vector2f& vec)
	{
		Element::setPosition(vec);
		m_shape.setPosition(vec);
	}

	void Rectangle::setRotation(float f)
	{
		m_shape.setRotation(f);
	}

	void Rectangle::setScale(const sf::Vector2f& vec)
	{
		m_shape.setScale(vec);
	}

	void Rectangle::setSize(const sf::Vector2f& vec)
	{
		m_shape.setSize(vec);
	}

	void Rectangle::setOrigin(const sf::Vector2f& vec)
	{
		m_shape.setOrigin(vec);
	}

	void Rectangle::updateFromCamera()
	{
		Element::updateFromCamera();
		m_shape.setPosition(getPosition());
	}

	void Rectangle::updateFromGame(NotificationManager::Dictionary dic)
	{
		float f = *static_cast<float*>(dic.m_dict["percentHP"]);
		setSizePercent(sf::Vector2f(f, 1));
	}
}