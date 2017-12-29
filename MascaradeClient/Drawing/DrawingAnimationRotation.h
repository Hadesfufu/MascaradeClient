////////////////////////////////////////////////////////////
// < Fichier : "DrawingAnimationRotation.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef DRAWINGANIMATIONROTATION_H
#define DRAWINGANIMATIONROTATION_H

#include "DrawingAnimation.h"

class DrawingAnimationRotation : public DrawingAnimation
{
public:
	DrawingAnimationRotation(sf::Time duration, sf::Transformable* elem, float angle);
	~DrawingAnimationRotation();

protected:
	void effect(float percent);

private:
	float m_InitialAngle;
	float m_FinishedAngle;
};

#endif 