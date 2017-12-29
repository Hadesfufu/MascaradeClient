
#include "DrawingAnimationRotation.h"

DrawingAnimationRotation::DrawingAnimationRotation(sf::Time duration, sf::Transformable* elem, float angle) : DrawingAnimation(duration, elem), m_FinishedAngle(angle - elem->getRotation())
{
	m_InitialAngle = elem->getRotation();
}
DrawingAnimationRotation::~DrawingAnimationRotation()
{

}

void DrawingAnimationRotation::effect(float percent){
	m_element->setRotation((percent*m_FinishedAngle) + m_InitialAngle);
}