#ifndef InputManagerBeta_H
#define InputManagerBeta_H
#include "Singleton.h"
#include <map>
#include "Data.h"
#include "j
#include <SFML/Window/Keyboard.hpp>

namespace Input {

	class Input
	{
	public:
		Input();
		~Input();

		virtual void load(nlohmann::json& j)
		{
			m_status = j.at("status");
		};

	private:
		std::string m_status;
	};

	class KeyboardInput : public Input
	{
		KeyboardInput();
		~KeyboardInput();

		void load(nlohmann::json& j)
		{
			m_alt = j.at("alt");
			m_shift = j.at("shift");
			m_ctrl = j.at("ctrl");
			m_windows = j.at("windows");
		}
		
	private: 
		bool m_alt, m_shift, m_ctrl, m_windows;
		sf::Keyboard::Key m_key;
		int language; 
	};

	class MouseInput : public Input
	{
		MouseInput();
		~MouseInput();

	private:
		bool m_alt, m_shift, m_ctrl, m_windows;
		sf::Keyboard::Key m_key;
	};

	class ManagerBeta : public Singleton<ManagerBeta>
	{
		friend class Singleton<ManagerBeta>;
	public:
		ManagerBeta();
		~ManagerBeta();

		void load()
		{
			std::string type;
			for(auto& actions: Data::json().at("InputMap"))
			{
				std::string action = actions.at("action");
				for(auto& input: actions.at("inputs"))
				{
					type = input.at("type");
					if (type == "keyboard")
					{

					}
					else if (type == "mouse")
					{

					}
					else
						Log::error("ManagerBeta-load") << "Input type not recognised";
				}
			}
		}
	private: 

		std::map<KeyboardInput, std::string> m_keyboardinputs;
		std::map<MouseInput, std::string> m_mouseinputs;
	};
}

#endif

