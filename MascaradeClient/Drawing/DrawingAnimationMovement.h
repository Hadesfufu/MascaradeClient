////////////////////////////////////////////////////////////
// < Fichier : "DrawingAnimationMovement.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef DRAWINGANIMATIONMOVEMENT_H
#define DRAWINGANIMATIONMOVEMENT_H

#include "DrawingAnimation.h"

class DrawingAnimationMovement : public DrawingAnimation
{
public:
	DrawingAnimationMovement(sf::Time duration, sf::Transformable* elem, sf::Vector2f& position);
	~DrawingAnimationMovement();

protected:
	void effect(float percent);

private:
	sf::Vector2f		m_StartingPosition;
	sf::Vector2f		m_FinishPosition;
};

#endif 