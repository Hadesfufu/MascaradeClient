
#include "Camera.h"
#include "NotificationManager.h"
#include "Log.h"
#include "Updater.h"

Camera::Camera(const sf::Vector2u windowSize)
: m_FocusedEnt(nullptr),
m_WindowSize(windowSize),
m_FollowMode(true),
m_Factor(-1.f)
{
	Updater::I()->add(this);
}

void Camera::onUpdate(const sf::Time& dt){
	if (!m_FocusedEnt) return;

	sf::Vector2f newpos;
	if (m_FollowMode){
		newpos = m_FocusedEnt->getPosition();
	}
	else{
		newpos = subRectAdjust();
	}
	clamp(newpos);
	if (newpos != getCenter()) {
		m_PreviousRelativePosition = getRelativePosition();
		setCenter(newpos);
		NotificationManager::I()->PostNotification("CameraUpdate");
	}
}

sf::Vector2f& Camera::clamp(sf::Vector2f& newpos){
	if (newpos.x < this->getSize().x / 2) newpos.x = this->getSize().x / 2;
	else if (newpos.x > m_MapSize.x - this->getSize().x / 2) newpos.x = (float)m_MapSize.x - this->getSize().x / 2;
	if (newpos.y < this->getSize().y / 2) newpos.y = this->getSize().y / 2;
	else if (newpos.y > m_MapSize.y - this->getSize().y / 2) newpos.y = (float)m_MapSize.y - this->getSize().y / 2;
	return newpos;
}

sf::Vector2f Camera::subRectAdjust(){
	sf::Vector2f entposR = m_FocusedEnt->getPosition() - (this->getCenter()-this->getSize()/2.f);
	sf::Vector2f move(0, 0);

	if (entposR.x < m_SubRect.left){
		move.x = entposR.x - m_SubRect.left;
	}
	else if (entposR.x > m_SubRect.left + m_SubRect.width){
		move.x = entposR.x - (m_SubRect.left + m_SubRect.width);
	}

	if (entposR.y < m_SubRect.top){
		move.y = entposR.y - m_SubRect.top;
	}
	else if (entposR.y > m_SubRect.top + m_SubRect.height){
		move.y = entposR.y - (m_SubRect.top + m_SubRect.height);
	}
	return move + getCenter();
}

// ---------Setters
void Camera::setFollow(bool follow){
	if (m_Factor == -1.f)
		m_FollowMode = true;
	else
		m_FollowMode = follow;
}

void Camera::setFactor(float fac){
	m_Factor = fac;

	m_SubRect.left = m_WindowSize.x/2 * m_Factor;
	m_SubRect.top = m_WindowSize.y/2 * m_Factor;
	m_SubRect.width = m_WindowSize.x * m_Factor;
	m_SubRect.height = m_WindowSize.y * m_Factor;

}

void Camera::focusOn(sf::Transformable& ent){
	m_FocusedEnt = &ent;
}
void Camera::setMapSize(const sf::Vector2f& size){
	m_MapSize = size;
}

// ---------- Getters

float Camera::getFactor(){
	return m_SubRect.width / m_MapSize.x;
}

sf::Vector2f Camera::getRelativePosition()
{
	sf::Vector2f position;
	position = getCenter();
	position.x -= (getSize().x / 2);
	position.y -= (getSize().y / 2);
	return position;
}

sf::Vector2f Camera::getPositionEvolution()
{
	return getRelativePosition() - m_PreviousRelativePosition;
}

