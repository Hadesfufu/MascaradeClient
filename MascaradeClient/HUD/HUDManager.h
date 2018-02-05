////////////////////////////////////////////////////////////
// < Fichier : "HUDManager.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef HUDManager_H
#define HUDManager_H
#include <string>
#include "zeraTeam_system.h"
#include "HUDContainer.h"
namespace HUD {
	class Manager
	{
	public:
		Manager();
		~Manager();

		void load();
		void clear();

		void update();
	private:
		sf::RenderWindow*  m_window;
		pugi::xml_document m_document;
		Container*		   m_root;
	};
}
#endif
