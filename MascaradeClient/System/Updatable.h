
#ifndef UPDATABLE_H
#define UPDATABLE_H

#include <SFML/Graphics.hpp>
class Updatable
{
public:
	Updatable();
	~Updatable();

	virtual	void onUpdate(const sf::Time& dt) = 0;
};

#endif