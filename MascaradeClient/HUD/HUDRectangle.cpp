#include "HUDRectangle.h"

namespace HUD
{
	Rectangle::Rectangle(Element* parent, json& node) : Element(parent, node)
	{
		load(node);
	}

	Rectangle::~Rectangle()
	{
		
	}
	
	void Rectangle::load(json& node)
	{
		sf::Color color;
		json& childLoader = node;
		std::string s;
		float f;
		
		/////////////////
		//FillColor
		/////////////////
		try{
		if (childLoader = node.at("fillColor")) {
			color.r = childLoader.at("r");
			color.g = childLoader.at("g");
			color.b = childLoader.at("b");
			color.a = childLoader.at("a");
			m_shape.setFillColor(color);
		}
		}
		catch (...) {}

		/////////////////
		//OutlineColor
		/////////////////
		try {
		if (childLoader = node.at("outlineColor")) {
			color.r = childLoader.at("r");
			color.g = childLoader.at("g");
			color.b = childLoader.at("b");
			color.a = childLoader.at("a");
			m_shape.setOutlineColor(color);
		}
		}
		catch (...) {}

		/////////////////
		//OutlineThickness
		/////////////////
		try {
			if (childLoader = node.at("outlineThickness")) {
				f = childLoader.at("Thickness");
				m_shape.setOutlineThickness(f);
			}
		}
		catch (...) {}

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