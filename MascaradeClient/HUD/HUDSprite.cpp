#include "HUDSprite.h"
namespace HUD
{
	Sprite::Sprite(Element* parent, pugi::xml_node& node) : Element(parent, node)
	{
		load(node);
	}
	Sprite::~Sprite()
	{
		
	}

	void Sprite::load(pugi::xml_node& node)
	{
		sf::IntRect rect;
		pugi::xml_node childLoader;
		sf::Color color;
		sf::Texture* texture;
		std::string string;
		/////////////////
		//Texture
		/////////////////
		if (childLoader = node.child("texture")) {
			string = childLoader.first_attribute().as_string();
			texture = TextureHolder::I()->getTexture(string);
			if (!texture)
				Log::error("HUDSprite") << string;
			m_Sprite.setTexture(*texture);
		}

		/////////////////
		//Texture Rect
		/////////////////
		if (childLoader = node.child("textureRect")) {
			rect.left = childLoader.attribute("left").as_int();
			rect.top = childLoader.attribute("top").as_int();
			rect.width = childLoader.attribute("width").as_int();
			rect.height = childLoader.attribute("height").as_int();
			m_Sprite.setTextureRect(rect);
		}

		/////////////////
		//Color
		/////////////////
		if (childLoader = node.child("color")) {
			color.r = childLoader.attribute("r").as_int();
			color.g = childLoader.attribute("g").as_int();
			color.b = childLoader.attribute("b").as_int();
			color.a = childLoader.attribute("a").as_int();
			m_Sprite.setColor(color);
		}
		m_Sprite.setOrigin(getOrigin());
		m_Sprite.setPosition(getPosition());
		setSize(sf::Vector2f(m_Sprite.getGlobalBounds().width, m_Sprite.getGlobalBounds().height));
		Drawer::I()->addToLayer("HUD", &m_Sprite);
	}

	void Sprite::setPosition(const sf::Vector2f& vec)
	{
		Element::setPosition(vec);
		m_Sprite.setPosition(getPosition());
	}

	void Sprite::setSize(const sf::Vector2f& vec)
	{
		//Element::setSize(vec);
	}

	void Sprite::setScale(const sf::Vector2f& vec)
	{
		m_Sprite.setScale(vec);
	}

	void Sprite::setRotation(float f)
	{
		m_Sprite.setRotation(f);
	}

	void Sprite::setOrigin(const sf::Vector2f& vec)
	{
		Element::setOrigin(vec);
		m_Sprite.setOrigin(getOrigin());
	}

	void Sprite::updateFromCamera()
	{
		Element::updateFromCamera();
		m_Sprite.setPosition(getPosition());
	}

}