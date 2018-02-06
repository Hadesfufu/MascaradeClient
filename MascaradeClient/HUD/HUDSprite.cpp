#include "HUDSprite.h"
namespace HUD
{
	Sprite::Sprite(Element* parent, json& node) : Element(parent, node)
	{
		load(node);
	}
	Sprite::~Sprite()
	{
		
	}

	void Sprite::load(json& node)
	{
		sf::IntRect rect;
		json& childLoader = node;
		sf::Color color;
		sf::Texture* texture;
		std::string string;
		/////////////////
		//Texture
		/////////////////
		try{
		texture = TextureHolder::I()->getTexture(node.at("texture"));
		if (!texture)
			Log::error("HUDSprite") << string;
		m_Sprite.setTexture(*texture);
		}
		catch (...) {}

		/////////////////
		//Texture Rect
		/////////////////
		try {
		if (childLoader = node.at("textureRect")) {
			rect.left = childLoader.at("left");
			rect.top = childLoader.at("top");
			rect.width = childLoader.at("width");
			rect.height = childLoader.at("height");
			m_Sprite.setTextureRect(rect);
		}
		}
		catch (...) {}

		/////////////////
		//Color
		/////////////////
		try {
			if (childLoader = node.at("color")) {
				color.r = childLoader.at("r");
				color.g = childLoader.at("g");
				color.b = childLoader.at("b");
				color.a = childLoader.at("a");
				m_Sprite.setColor(color);
			}
		}
		catch (...) {}
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