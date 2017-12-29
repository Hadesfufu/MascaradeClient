
#include "DrawingAnimation.h"
#include "Drawer.h"

DrawingAnimation::DrawingAnimation(sf::Time duration, sf::Transformable* drawable)
	: m_element(drawable)
	, m_duration(duration)
	, m_isAlive(true)
	, m_easing(linear)
	, m_delay(sf::seconds(0)){
}
/*
DrawingAnimation::DrawingAnimation(sf::Time duration, int Layer) 
	: m_layer(&Drawer::I()->getLayer(Layer)->transformable)
	, m_duration(duration)
	, m_isAlive(true)
	, m_easing(linear)
	, m_delay(sf::seconds(0)){
}*/

DrawingAnimation::~DrawingAnimation(){
};

void DrawingAnimation::update(){
	if (m_clock.getElapsedTime() > m_delay && m_element){
		if (m_clock.getElapsedTime() < (m_duration + m_delay)){
			float percent = (float)calculatePercent();
			if (m_comebacking)
				percent = 1.f - percent;
			effect(percent);
		}
		else{
			if (m_looping)
			{
				m_clock.restart();
				if (m_comebackloop)
				{
					m_comebacking = !m_comebacking;
				}
			}
			else
				m_isAlive = false;
		}
	}
}

double DrawingAnimation::calculatePercent(){
	float currentTime = (m_clock.getElapsedTime() - m_delay).asSeconds();
	float basicPercent = (currentTime / m_duration.asSeconds());
	double s = 1.70158;
	switch (m_easing){
	case linear:
		return basicPercent;
		break;
	case easeInQuad:
		return	basicPercent * basicPercent;
		break;
	case easeOutQuad:
		return -basicPercent*(basicPercent - 2);
		break;
	case easeInOutQuad:
		if (basicPercent < 0.5)
			return basicPercent*basicPercent*2;
		else
			return  (-basicPercent+0.5)*(basicPercent - 2)+0.5;
		break;

	case easeInCubic:
		return	basicPercent * basicPercent * basicPercent;
		break;
	case easeOutCubic:
		basicPercent--;
		return basicPercent * basicPercent * basicPercent+1;
		break;
	case easeInOutCubic:
		if (basicPercent < 0.5)
			return basicPercent*basicPercent*basicPercent * 4;
		else{
			basicPercent -= 1.5;
			return  ((basicPercent * basicPercent * basicPercent -2) + 2) / 2;
		}
		break;

	case easeInQuart:
		return	basicPercent * basicPercent * basicPercent * basicPercent;
		break;
	case easeOutQuart:
		basicPercent--;
		return - (basicPercent * basicPercent * basicPercent * basicPercent - 1);
		break;
	case easeInOutQuart:
		if (basicPercent < 0.5)
			return basicPercent*basicPercent*basicPercent*basicPercent * 8;
		else{
			basicPercent -= 1.5;
			return  -((basicPercent * basicPercent* basicPercent * basicPercent - 2) / 2);
		}
		break;

	case easeInQuint:
		return	basicPercent * basicPercent * basicPercent * basicPercent * basicPercent;
		break;
	case easeOutQuint:
		basicPercent--;
		return (basicPercent * basicPercent * basicPercent * basicPercent * basicPercent + 1);
		break;
	case easeInOutQuint:
		if (basicPercent < 0.5)
			return basicPercent*basicPercent*basicPercent*basicPercent*basicPercent * 16;
		else{
			basicPercent -= 1.5;
			return  ((basicPercent * basicPercent * basicPercent* basicPercent * basicPercent + 2) / 2);
		}
		break;
	case easeInCirc:
		return -(sqrtf(1 - basicPercent*basicPercent) - 1);
		break;
	case easeOutCirc:
		basicPercent--;
		return sqrtf(1 - basicPercent*basicPercent);
		break;
	case easeInOutCirc:
		basicPercent *= 2;
		if (basicPercent < 1)
			return -(sqrtf(1 - basicPercent*basicPercent) - 1) / 2;
		else{
			basicPercent -= 2;
			return  (sqrtf(1 - basicPercent*basicPercent) + 1) / 2;
		}
		break;

	case easeInExpo:
		return (currentTime == 0) ? 0 : pow(2, 10 * (basicPercent - 1));
		break;
	case easeOutExpo:
		return (basicPercent == 1) ? 1: (-pow(2, -10 * basicPercent) + 1);
		break;
	case easeInOutExpo:
		if (currentTime == 0) return 0;
		else if (basicPercent == 1) return 1;
		else if ((basicPercent *= 2) < 1) return pow(2, 10 * (basicPercent - 1)) / 2;
		else return (-pow(2, -10 * (basicPercent-1)) + 2) / 2;
		break;

	case easeInBack:
		return basicPercent*basicPercent*((s + 1)*basicPercent - s);
		break;
	case easeOutBack:
		basicPercent--;
		return (basicPercent*basicPercent*((s + 1)*basicPercent + s) + 1);
		break;
	case easeInOutBack:
		if ((basicPercent *= 2) < 1) return  (basicPercent*basicPercent*(((s *= (1.525)) + 1)*basicPercent- s)) / 2;
		return ((basicPercent -= 2)*basicPercent*(((s *= (1.525)) + 1)*basicPercent + s) + 2) / 2;
		break;

	default:
		return (m_clock.getElapsedTime() - m_delay) / m_duration;
		break;
	}
}