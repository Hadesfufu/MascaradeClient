#ifndef InputManagerBeta_H
#define InputManagerBeta_H
#include "Singleton.h"
#include <map>
#include "Data.h"
#include "json.hpp"
#include <SFML/Graphics.hpp>

namespace Input {

	class Input
	{
	public:
		Input(json& j) { load(j); };
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
	public: 
		KeyboardInput(json& j) : Input(j) {};
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
		sf::Event::EventType m_type;
		int language; 
	};

	class MouseInput : public Input
	{
		MouseInput(json& h) : Input(j) {};
		~MouseInput();

	private:

		sf::Mouse::Button m_key;
	};

	class ManagerBeta : public Singleton<ManagerBeta>
	{
		friend class Singleton<ManagerBeta>;
	public:
		ManagerBeta() { load(); };
		~ManagerBeta();

		void addEventTrigger(sf::Event::EventType type, std::string& string) {
			m_eventTrigger.emplace(type, string);
		}

		void load()
		{
			for(auto& actions: Data::json().at("InputMap"))
			{
				std::string action = actions.at("action");
				for(auto& input: actions.at("inputs"))
				{
					std::string type = input.at("type");
					if (type == "keyboard")
					{
						m_keyboardinputs.emplace(KeyboardInput(input), action);
					}
					else if (type == "mouse")
					{
						m_mouseinputs.emplace(MouseInput(input), action);
					}
					else
						Log::error("ManagerBeta-load") << "Input type not recognised";
				}
			}
		}
	private: 

		std::map<sf::Event::EventType, std::string> m_eventTrigger; 
		std::map<KeyboardInput, std::string>		m_keyboardinputs;
		std::map<MouseInput, std::string>			m_mouseinputs;
	};
}

#endif

