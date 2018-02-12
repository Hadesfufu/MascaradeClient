#ifndef InputManagerBeta_H
#define InputManager_H
#include "Singleton.h"
#include <map>
#include "Data.h"
#include "json.hpp"
#include <SFML/Graphics.hpp>
#include <NotificationManager.h>

namespace Input {
	using json = nlohmann::json;

	class Action {
	public:
		Action(std::string& str) {
			m_actionString = str;
		}

		~Action() {

		}

		void launch(sf::Event& e) {
			// todo : add test.
			if (!m_enabeled)
				return;

			NotificationManager::Dictionary dico;
			dico.m_dict.emplace("event",&e);
			NotificationManager::I()->PostNotification(m_actionString, dico);
		}

	private:
		std::string m_actionString;
		bool m_enabeled = true;
	};

	
	class Manager : public Singleton<Manager>
	{
		friend class Singleton<Manager>;
	public:
		Manager() { load(); }
		~Manager() {
		}

		void addEventTrigger(sf::Event::EventType type, std::string action) {
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, Action>());
			m_events.at(type).emplace(-1, action);
		}

		void addKeyboardTrigger(sf::Event::EventType type, sf::Keyboard::Key key, std::string action) {
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, Action>());
			m_events.at(type).emplace(key, action);
		}


		void addMouseTrigger(sf::Event::EventType type, sf::Mouse::Button button, std::string action) {
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, Action>());
			m_events.at(type).emplace(button, action);
		}

		void handleEvent(sf::Event& event) {
			try {
				if (event.type == 5 || event.type == 6)
					m_events.at(event.type).at(event.key.code).launch(event);
				else if (event.type >= 7 && event.type <= 13)
					m_events.at(event.type).at(event.mouseButton.button).launch(event);
				else if (event.type >= 14 && event.type <= 22)
					m_events.at(event.type).at(event.joystickButton.button).launch(event);
				else
					m_events.at(event.type).at(-1).launch(event);
			}
			catch (...) {
			}
		}

		void load()
		{
			std::string action;
			sf::Event::EventType type;
			for (auto& actions : Data::json().at("InputMap"))
			{
				action = actions.at("action").get<std::string>();
				for (auto& input : actions.at("inputs"))
				{
					type = sf::Event::EventType(input.at("type").get<int>());
					if (type == sf::Event::EventType::Count)
						continue; 

					if(m_events.find(type) == m_events.end())
						m_events.emplace(type, std::map<int, Action>());
				
					m_events.at(type).emplace(input.at("key").get<int>(), action);
				}
			}
		}

		
	private:
		std::map<sf::Event::EventType, std::map<int, Action>>		m_events;
	};
}

#endif

