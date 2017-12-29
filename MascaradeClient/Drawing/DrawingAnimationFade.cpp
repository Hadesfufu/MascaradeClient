
#include "DrawingAnimationFade.h"
#include "Log.h"

DrawingAnimationFade::DrawingAnimationFade(sf::Time duration, sf::Sprite* elem, FadeType ft): DrawingAnimation(duration, elem), m_FadeType(ft), m_elemS(elem)
{
}

DrawingAnimationFade::DrawingAnimationFade(sf::Time duration, sf::Text* elem, FadeType ft) : DrawingAnimation(duration, elem), m_FadeType(ft),  m_elemT(elem)
{
}

DrawingAnimationFade::DrawingAnimationFade(sf::Time duration, sf::RectangleShape* elem, FadeType ft) : DrawingAnimation(duration, elem), m_FadeType(ft), m_elemRS(elem)
{
}

DrawingAnimationFade::DrawingAnimationFade(sf::Time duration, sf::CircleShape* elem, FadeType ft) : DrawingAnimation(duration, elem), m_FadeType(ft), m_elemCS(elem)
{
}

DrawingAnimationFade::DrawingAnimationFade(sf::Time duration, sf::ConvexShape* elem, FadeType ft) : DrawingAnimation(duration, elem), m_FadeType(ft), m_elemConvS(elem)
{
}

DrawingAnimationFade::~DrawingAnimationFade()
{
}

void DrawingAnimationFade::effect(float percent){

	if (m_elemS){
		sf::Color color = m_elemS->getColor();
		changeColor(color, percent);
		m_elemS->setColor(color);
	}
	else if(m_elemT)
	{
		sf::Color color = m_elemT->getFillColor();
		sf::Color color2 = m_elemT->getOutlineColor();
		changeColor(color, percent);
		changeColor(color2, percent);
		m_elemT->setFillColor(color);
		m_elemT->setOutlineColor(color2);
	}
	else if (m_elemRS)
	{
		sf::Color color = m_elemRS->getFillColor();
		sf::Color color2 = m_elemRS->getOutlineColor();
		changeColor(color, percent);
		changeColor(color2, percent);
		m_elemRS->setFillColor(color);
		m_elemRS->setOutlineColor(color2);
	}
	else if (m_elemCS)
	{
		sf::Color color = m_elemCS->getFillColor();
		sf::Color color2 = m_elemCS->getOutlineColor();
		changeColor(color, percent);
		changeColor(color2, percent);
		m_elemCS->setFillColor(color);
		m_elemCS->setOutlineColor(color2);
	}
	else if (m_elemConvS)
	{
		sf::Color color = m_elemConvS->getFillColor();
		sf::Color color2 = m_elemConvS->getOutlineColor();
		changeColor(color, percent);
		changeColor(color2, percent);
		m_elemConvS->setFillColor(color);
		m_elemConvS->setOutlineColor(color2);
	}
	else
	{
		Log::error("Drawing Animation Fade") << "The element must have disapeared";
	}
}

void DrawingAnimationFade::changeColor(sf::Color& color, float percent)
{
	if (m_FadeType == FadeIn)
		color.a = (unsigned int)(percent * 255);
	else
		color.a = (unsigned int)((1.0f - percent) * 255);
}
