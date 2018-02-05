#include "HUDText.h"
namespace HUD {
	HUD::Text::Text(Element* parent, pugi::xml_node& node) : Element(parent)
	{
		load(node);
	}

	HUD::Text::~Text()
	{
		NotificationManager::I()->RemoveObserver(this);
	}

	void HUD::Text::load(pugi::xml_node& node)
	{
		sf::Color		color;
		sf::Vector2f	vec;
		float			f;
		std::string		s;
		pugi::xml_node childLoader;

		/////////////////
		//FillColor
		/////////////////
		if (childLoader = node.child("fillColor")) {
			color.r = childLoader.attribute("r").as_int();
			color.g = childLoader.attribute("g").as_int();
			color.b = childLoader.attribute("b").as_int();
			color.a = childLoader.attribute("a").as_int(); 
			m_text.setFillColor(color);
		}

		/////////////////
		//OutlineColor
		/////////////////
		if (childLoader = node.child("outlineColor")) {
			color.r = childLoader.attribute("r").as_int();
			color.g = childLoader.attribute("g").as_int();
			color.b = childLoader.attribute("b").as_int();
			color.a = childLoader.attribute("a").as_int();
			m_text.setOutlineColor(color);
		}

		/////////////////
		//OutlineThickness
		/////////////////
		if (childLoader = node.child("outlineThickness")) {
			f = childLoader.first_attribute().as_float();
			m_text.setOutlineThickness(f);
		}

		/////////////////
		//Font Size
		/////////////////
		if (childLoader = node.child("charSize"))
		{
			m_text.setCharacterSize(childLoader.first_attribute().as_uint());
		}
	
		/////////////////
		//Font
		/////////////////
		if (childLoader = node.child("font"))
		{
			s = childLoader.first_attribute().as_string();
			m_text.setFont(*FontHolder::I()->getFont(s));
		}

		/////////////////
		//String
		/////////////////
		if(childLoader = node.child("string"))
		{
			s = childLoader.first_attribute().as_string();
			m_text.setString(s);
		}

		/////////////////
		//Style
		/////////////////
		if (childLoader = node.child("style"))
		{
			int style = 0;
			if (childLoader.attribute("bold").as_bool())
				style += 1;
			if (childLoader.attribute("italic").as_bool())
				style += 2;
			if (childLoader.attribute("underline").as_bool())
				style += 4;
			m_text.setStyle(style);
		}

		/////////////////
		//Action
		/////////////////
		if (childLoader = node.child("action"))
		{
			s = childLoader.first_attribute().as_string();
			NotificationManager::I()->AddObserver(s, this, &Text::setViaNotif);
		}
		
		setBounds(m_text.getGlobalBounds());
		loadElement(node);
		m_text.setPosition(getPosition());
		m_text.setOrigin(getOrigin());

		Drawer::I()->addToLayer("HUD", &m_text);
	}

	void Text::setViaNotif(NotificationManager::Dictionary d)
	{
		std::string s = *static_cast<std::string*>(d.m_dict["fps"]);
		setText(s);
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