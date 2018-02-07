#include "HUDText.h"
namespace HUD {
	HUD::Text::Text(Element* parent, json& node) : Element(parent)
	{
		load(node);
	}

	HUD::Text::~Text()
	{
		NotificationManager::I()->RemoveObserver(this);
	}

	void HUD::Text::load(json& node)
	{
		sf::Color		color;
		sf::Vector2f	vec;
		float			f;
		std::string		s;
		json& childLoader = node;
		
		/////////////////
		//FillColor
		/////////////////
		try {
			if (childLoader = node.at("fillColor")) {
				color.r = childLoader.at("r");
				color.g = childLoader.at("g");
				color.b = childLoader.at("b");
				color.a = childLoader.at("a");
				m_text.setFillColor(color);
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
				m_text.setOutlineColor(color);
			}
		}
		catch (...) {}

		/////////////////
		//OutlineThickness
		/////////////////
		try {
			m_text.setOutlineThickness(node.at("outlineThickness"));
		}
		catch (...) {}

		/////////////////
		//Font Size
		/////////////////
		try {
			m_text.setCharacterSize(node.at("charSize"));
		}
		catch (...) {}

		/////////////////
		//Font
		/////////////////
		try {
			m_text.setFont(*FontHolder::I()->getFont(node.at("font")));

		}
		catch (...) {}

		/////////////////
		//String
		/////////////////
		try {
			std::string s = node.at("string");
			m_text.setString(sf::String(s));
		}
		catch (...) {}

		/////////////////
		//Style
		/////////////////
		try {
			if (childLoader = node.at("style"))
			{
				int style = 0;
				if (childLoader.at("bold"))
					style += 1;
				if (childLoader.at("italic"))
					style += 2;
				if (childLoader.at("underline"))
					style += 4;
				m_text.setStyle(style);
			}
		}
		catch (...) {}

		setBounds(m_text.getGlobalBounds());
		loadElement(node);
		m_text.setPosition(getPosition());
		m_text.setOrigin(getOrigin());

		Drawer::I()->addToLayer("HUD", &m_text);
	}

	void Text::setRotation(float f)
	{
		m_text.setRotation(f);
	}

	void Text::setScale(const sf::Vector2f& vec)
	{
		m_text.setScale(vec);
	}

	void Text::setText(std::string s)
	{
		m_text.setString(s);
	}

	void Text::updateFromCamera()
	{
		Element::updateFromCamera();
		m_text.setPosition(getPosition());
	}


}