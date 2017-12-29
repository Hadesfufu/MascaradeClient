////////////////////////////////////////////////////////////
// < Fichier : "DrawingAnimationFade.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef DRAWINGANIMATIONFADE_H
#define DRAWINGANIMATIONFADE_H

#include "DrawingAnimation.h"

enum FadeType
{ 
	FadeOut, 
	FadeIn 
};

class DrawingAnimationFade : public DrawingAnimation
{
public:
	DrawingAnimationFade(sf::Time duration, sf::Sprite* elem, FadeType ft);
	DrawingAnimationFade(sf::Time duration, sf::Text* elem, FadeType ft);
	DrawingAnimationFade(sf::Time duration, sf::RectangleShape* elem, FadeType ft);
	DrawingAnimationFade(sf::Time duration, sf::CircleShape* elem, FadeType ft);
	DrawingAnimationFade(sf::Time duration, sf::ConvexShape* elem, FadeType ft);
	~DrawingAnimationFade();

	void					effect(float percent);
	void					changeColor(sf::Color& color, float percent);

	FadeType				getFadeType() { return m_FadeType; }

private: 
	FadeType				m_FadeType;

	sf::Sprite*				m_elemS = nullptr;
	sf::Text*				m_elemT = nullptr;
	sf::RectangleShape*		m_elemRS = nullptr;
	sf::CircleShape*		m_elemCS = nullptr;
	sf::ConvexShape*		m_elemConvS = nullptr;
};

#endif 