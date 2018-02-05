#include "HUDElement.h"
#include "Application.h"

namespace HUD {
	HUD::Element::Element(Element* parent, pugi::xml_node& node)
		: m_parent(parent),
		m_Bounds(0, 0, 0, 0),
		m_Origin(0, 0),
		m_clip(false),
		m_Padding(0, 0, 0, 0),
		m_Margin(0, 0, 0, 0)
	{
		if (!parent)
			setSize(Drawer::I()->getCamera()->getSize());
		loadElement(node);
	}

	Element::Element(Element* parent)
		:m_parent(parent),
		m_Bounds(0, 0, 0, 0),
		m_Origin(0, 0),
		m_clip(false),
		m_Padding(0, 0, 0, 0),
		m_Margin(0, 0, 0, 0)
	{
		
	}


	HUD::Element::~Element()
	{
	}

	void Element::loadElement(pugi::xml_node& node)
	{
		sf::FloatRect	frect;
		sf::Vector2f	vec;
		pugi::xml_node	childLoader;

		/////////////////
		//Margin
		/////////////////
		if (childLoader = node.child("margin")) {
			frect.left = childLoader.attribute("left").as_float();
			frect.top = childLoader.attribute("top").as_float();
			frect.width = childLoader.attribute("width").as_float();
			frect.height = childLoader.attribute("height").as_float();
			setMargin(frect);
		}

		/////////////////
		//Padding
		/////////////////
		if (childLoader = node.child("padding")) {
			frect.left = childLoader.attribute("left").as_float();
			frect.top = childLoader.attribute("top").as_float();
			frect.width = childLoader.attribute("width").as_float();
			frect.height = childLoader.attribute("height").as_float();
			setPadding(frect);
		}
		
		/////////////////
		//Origin
		/////////////////
		if (childLoader = node.child("origin")) {
			vec.x = childLoader.attribute("x").as_float();
			vec.y = childLoader.attribute("y").as_float();
			setOrigin(vec);
		}

		/////////////////
		//Position
		/////////////////
		vec.x = 0; vec.y = 0;
		if (childLoader = node.child("position")) {
			vec.x = childLoader.attribute("x").as_float();
			vec.y = childLoader.attribute("y").as_float();
		}
		setPosition(vec);


		/////////////////
		//Size
		/////////////////
		if (childLoader = node.child("size")) {
			vec.x = childLoader.attribute("x").as_float();
			vec.y = childLoader.attribute("y").as_float();
			setSize(vec);
		}

		/////////////////
		//SizePercent
		/////////////////
		if (childLoader = node.child("sizePercent")) {
			vec.x = childLoader.attribute("x").as_float();
			vec.y = childLoader.attribute("y").as_float();
			setSizePercent(vec);
		}

		/////////////////
		//FloatRight
		/////////////////
		if (childLoader = node.child("floatRight")) {
			setFloatRight();
		}
		
		/////////////////
		//FloatRight
		/////////////////
		if (childLoader = node.child("floatBot")) {
			setFloatBot();
		}
		/////////////////
		//PositionPercent
		/////////////////
		if (childLoader = node.child("positionPercent")) {
			vec.x = childLoader.attribute("x").as_float();
			vec.y = childLoader.attribute("y").as_float();
			setPositionPercent(vec);
		}

	}


	void HUD::Element::setPosition(const sf::Vector2f& vec)
	{
		sf::Vector2f origin(0, 0);
		sf::FloatRect parentPadding(0, 0, 0, 0);
		if (checkParent()) {
			origin = m_parent->getPosition();
			parentPadding = m_parent->getPadding();
		}
		m_Bounds.left = origin.x + vec.x + m_Origin.x + m_Margin.width + parentPadding.left;
		m_Bounds.top = origin.y + vec.y + m_Origin.y + m_Margin.height + parentPadding.top;
	}


	void HUD::Element::setPositionPercent(sf::Vector2f& vec)
	{
		if (!checkParent())
			return;
		sf::Vector2f	newPosition;
		sf::Vector2f	parentSize = m_parent->getSize();
		newPosition.x = (parentSize.x * vec.x);
		newPosition.y = (parentSize.y * vec.x);
		setPosition(newPosition);
	}

	void HUD::Element::setSize(const sf::Vector2f& vec)
	{
		if(!checkParent())
		{
			m_Bounds.width = vec.x;
			m_Bounds.height = vec.y;
			return;
		}

		if (vec.x < 0)
			m_Bounds.width = 0;
		else if (m_Bounds.left + vec.x > m_parent->getAvailableArea().left + m_parent->getAvailableArea().width - m_Margin.width)
			m_Bounds.width = m_parent->getAvailableArea().width - (m_Bounds.left - m_parent->getAvailableArea().left) - m_Margin.width;
		else
			m_Bounds.width = vec.x;

		if (vec.y < 0)
			m_Bounds.height = 0;
		else if (m_Bounds.top + vec.y > m_parent->getAvailableArea().top + m_parent->getAvailableArea().height - m_Margin.height)
			m_Bounds.height = m_parent->getAvailableArea().height - (m_Bounds.top - m_parent->getAvailableArea().top) - m_Margin.height;
		else
			m_Bounds.height = vec.y;
	}

	void HUD::Element::setSizePercent(sf::Vector2f& vec)
	{
		if (!checkParent())
			return;
		sf::Vector2f	newSize;
		newSize.x = m_parent->getSize().x * vec.x;
		newSize.y = m_parent->getSize().y * vec.y;
		setSize(newSize);
	}

	void Element::setMargin(sf::FloatRect& margin)
	{
		m_Margin = margin;
		m_Bounds.left += margin.left;
		m_Bounds.top += margin.top;
		m_Bounds.width -= margin.width;
		m_Bounds.height -= margin.height;
	}

	void Element::setOriginPercent(const sf::Vector2f& vec)
	{
		sf::Vector2f newOrigin;
		newOrigin.x = getSize().x * vec.x;
		newOrigin.y = getSize().y * vec.y;
		setOrigin(newOrigin);
	}

	void Element::setFloatRight()
	{
		if (!checkParent())
			return;
		sf::FloatRect fr = m_parent->getAvailableArea();
		m_Bounds.left = fr.left + fr.width - m_Bounds.width;
	}

	void Element::setFloatBot()
	{
		if (!checkParent())
			return;
		sf::FloatRect fr = m_parent->getAvailableArea();
		m_Bounds.top = fr.top + fr.height - m_Bounds.height;		
	}

	bool HUD::Element::checkParent()
	{
		if (!m_parent)
		{
			Log::error("HUD-Element") << "no parent declared";
			return false;
		}
		return true;
	}

	sf::FloatRect HUD::Element::getAvailableArea()
	{
		sf::FloatRect area;
		area.left = getPosition().x + m_Padding.left;
		area.top = getPosition().y + m_Padding.top;
		area.width = getSize().x - m_Padding.width;
		area.height = getSize().y - m_Padding.height;

		return area;
	}

	void Element::clip()
	{
		if (!checkParent())
			return;
		sf::FloatRect frect = m_parent->getAvailableArea();
		//not used for now
	}

	void Element::updateFromCamera()
	{
		sf::Vector2f vec = Drawer::I()->getCamera()->getPositionEvolution();
		m_Bounds.left += vec.x;
		m_Bounds.top += vec.y;
	}
}
