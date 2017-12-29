
#include "DrawingAnimationMovement.h"

DrawingAnimationMovement::DrawingAnimationMovement(sf::Time duration, sf::Transformable* elem, sf::Vector2f& position) : DrawingAnimation(duration, elem), m_FinishPosition(position - elem->getPosition())
{
	m_StartingPosition = elem->getPosition();
}
/*DrawingAnimationMovement::DrawingAnimationMovement(sf::Time duration, int Layer, sf::Vector2f& position) : DrawingAnimation(duration, Layer), m_FinishPosition(position){
	if (m_layer->drawables.empty())
		m_StartingPosition = sf::Vector2f(0,0);
	else{
		
	}
}*/
DrawingAnimationMovement::~DrawingAnimationMovement()
{

}


void DrawingAnimationMovement::effect(float percent){
		m_element->setPosition((percent*m_FinishPosition ) + m_StartingPosition);
}