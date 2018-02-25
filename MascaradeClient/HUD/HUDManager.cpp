#include "HUDManager.h"
#include "../MascaradeClient.h"

namespace HUD {

	Manager::Manager()
	{
	}

	Manager::~Manager()
	{
		if (m_root)
			delete m_root;
		NotificationManager::I()->RemoveObserver(m_root);
	}

	void HUD::Manager::load(std::string s, sf::Vector2f& vec)
	{
		////////////////////////////////////////////////////////////
		// Load document
		////////////////////////////////////////////////////////////
		m_root = new Container(nullptr, Data::json().at(s));
		m_root->setSize(vec);
		NotificationManager::I()->AddObserver("CameraUpdate", m_root, &Container::updateFromCamera);
		m_root->updateFromCamera();
		Log::info("HUDManager") << "---------------- HUD Loading " << " : Sucess -----------------";
	}

	void Manager::clear()
	{
		delete m_root;
	}

}
