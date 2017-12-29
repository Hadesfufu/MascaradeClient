
#include "DrawingAnimationScale.h"

DrawingAnimationScale::DrawingAnimationScale(sf::Time duration, sf::Transformable* elem, sf::Vector2f& position) : DrawingAnimation(duration, elem), m_FinishScale(position - elem->getScale())
{
	m_StartingScale = elem->getScale();
}
/*DrawingAnimationMovement::DrawingAnimationMovement(sf::Time duration, int Layer, sf::Vector2f& position) : DrawingAnimation(duration, Layer), m_FinishPosition(position){
if (m_layer->drawables.empty())
m_StartingPosition = sf::Vector2f(0,0);
else{

}
}*/
DrawingAnimationScale::~DrawingAnimationScale()
{

}

void DrawingAnimationScale::effect(float percent){
	m_element->setScale((percent*m_FinishScale) + m_StartingScale);
}