////////////////////////////////////////////////////////////
// < Fichier : "LoadingScreen.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#if __TRACKING_LEAK
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include "Singleton.h"
#include "AnimatedEntity.h"
#include "FontHolder.h"
#include "windowController.h"

class LoadingScreen : public Singleton<LoadingScreen>, public WindowController
{
	friend class Singleton<LoadingScreen>;
public:
	LoadingScreen();
	~LoadingScreen();
	
	void Launch();
	void Stop();

	void t_func();

	void drawFrame(){};
	void setIsCurrentController(bool);
private:
	sf::Text		m_Text;
	bool			m_Continue;
	sf::Thread		m_LoadingThread;
	sf::Mutex		m_Mutex;
};


#endif 