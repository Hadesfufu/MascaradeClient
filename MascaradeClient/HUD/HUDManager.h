////////////////////////////////////////////////////////////
// < Fichier : "HUDManager.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef HUDManager_H
#define HUDManager_H
#include <string>
#include "HUDContainer.h"
#include "NotificationManager.h"
namespace HUD {
	class Manager
	{
	public:
		Manager();
		~Manager();

		void load(std::string s, sf::Vector2f& windowSize);
		void clear();

		void update();
	private:
		Container*		   m_root;
	};
}
#endif
