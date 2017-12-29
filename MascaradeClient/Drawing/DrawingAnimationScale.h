////////////////////////////////////////////////////////////
// < Fichier : "DrawingAnimationScale.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef DRAWINGANIMATIONSCALE_H
#define DRAWINGANIMATIONSCALE_H

#include "DrawingAnimation.h"

class DrawingAnimationScale : public DrawingAnimation
{
public:
	DrawingAnimationScale(sf::Time duration, sf::Transformable* elem, sf::Vector2f& scale);
	~DrawingAnimationScale();

protected:
	void effect(float percent);

private:
	sf::Vector2f		m_StartingScale;
	sf::Vector2f		m_FinishScale;
};

#endif 