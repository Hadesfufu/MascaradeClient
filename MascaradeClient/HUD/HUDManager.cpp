#include "HUDManager.h"
#include "Application.h"

namespace HUD {

	Manager::Manager():m_window(Application::I()->getWindow())
	{
	}

	Manager::~Manager()
	{
		if (m_root)
			delete m_root;
		NotificationManager::I()->RemoveObserver(m_root);
	}

	void HUD::Manager::load()
	{
		const std::string* path;
		path = Parameters::I()->getString("HUD");
		if (!path)
		{
			Log::error("HUDManager") << "Parameter not declared";
			return;
		}


		////////////////////////////////////////////////////////////
		// Load document
		////////////////////////////////////////////////////////////
		if (!m_document.load_file(path->c_str())) {
			Log::error("HUDManager") << "Unable to open the file. Path :" + *path;
			Log::info("HUDManager") << "---------------- HUD Loading " << " : Fail -----------------";
#pragma message("\ntodo : handle error\n")
			return;
		}
		m_root = new Container(nullptr, m_document.first_child());
		m_root->setSize(sf::Vector2f(m_window->getSize()));
		NotificationManager::I()->AddObserver("CameraUpdate", m_root, &Container::updateFromCamera);
		m_root->updateFromCamera();
		Log::info("HUDManager") << "---------------- HUD Loading " << " : Sucess -----------------";
	}

	void Manager::clear()
	{
		delete m_root;
	}

}
