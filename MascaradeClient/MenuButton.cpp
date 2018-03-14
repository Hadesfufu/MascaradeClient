#include "MenuButton.h"
#include "FontHolder.h"


MenuButton::~MenuButton()
{
}

void MenuButton::load(nlohmann::json& j)
{
	try
	{
		m_background.setTexture(*TextureHolder::I()->getTexture(j.at("texture")));
		m_text.setFont(*FontHolder::I()->getFont(j.at("font")));
		if (Data::exist(j, "charSize"))
			m_text.setCharacterSize(j.at("charSize"));
		if (Data::exist(j, "color"))
			m_text.setFillColor(Data::get<sf::Color>(j.at("color")));
		m_text.setString((Data::get<sf::String>(j.at("text"))));
		m_background.setOrigin(0, m_background.getGlobalBounds().height / 2);
		m_text.setOrigin(0, m_text.getLocalBounds().height / 2);
		if (Data::exist(j, "action"))
			setAction(j.at("action"));
	}catch(...)
	{
		Log::error("MenuButton::load") << "Button is badly done";
	}
}

void MenuButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background, states);
	target.draw(m_text, states);
}

void MenuButton::setPosition(sf::Vector2f position)
{
	m_text.setPosition(position + sf::Vector2f(10, -10));
	m_background.setPosition(position);
}

void MenuButton::setAction(std::string s) {
	Input::Manager::I()->addMouseTrigger(sf::Event::MouseButtonPressed, sf::Mouse::Button::Left, s, &m_background, (sf::Vector2f) m_background.getTexture()->getSize());
}
