#ifndef __Camera_H__
#define __Camera_H__

#include <SFML/Graphics.hpp>
#include "Updatable.h"


class Camera : public sf::View, public Updatable
{
public: 
	Camera(const sf::Vector2u = sf::Vector2u());

	void					focusOn(sf::Transformable& entity); // select an entity on which one you wanna focus the camera
	void					onUpdate(const sf::Time& dt);	// basic updateFunction

	/////////////////////////////////
	//Setters
	/////////////////////////////////
	void					setFollow(bool follow);
	void					setFactor(float factor);
	void					setMapSize(const sf::Vector2f& MapSize);

	/////////////////////////////////
	//Getters
	/////////////////////////////////
	float					getFactor();
	sf::Vector2f			getRelativePosition();

	//delta over 2frames 
	sf::Vector2f			getPositionEvolution();
private: //fonctions
	sf::Vector2f&			clamp(sf::Vector2f& newpos);
	sf::Vector2f			subRectAdjust();
	
private: //attributs
	sf::Transformable*		m_FocusedEnt;
	sf::Vector2f			m_MapSize;
	sf::Vector2f			m_PreviousRelativePosition;
	sf::Vector2u			m_WindowSize;
	sf::FloatRect			m_SubRect;
	float					m_Factor;
	bool					m_FollowMode;
};

#endif