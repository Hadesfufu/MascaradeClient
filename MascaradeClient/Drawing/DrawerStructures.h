////////////////////////////////////////////////////////////
// < Fichier : "DrawerStructures.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef DRAWERSTRUCTURES_H
#define DRAWERSTRUCTURES_H

#include <SFML/Graphics.hpp>

namespace DrawerStructures{
	struct Drawable{
		Drawable::Drawable(const sf::Drawable* drawable, sf::Shader* shader = nullptr, bool hidden = false) : drawable(drawable), hidden(hidden), shader(shader){}
		const sf::Drawable*		drawable;
		bool					hidden;
		sf::Shader*				shader;
	};

	struct Z_index{
		std::vector<Drawable>	drawables;
		bool					hidden = false;
	};
}

#endif 