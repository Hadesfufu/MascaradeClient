#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "Singleton.h"
#include <map>
#include "Data.h"
#include "json.hpp"
#include <SFML/Graphics.hpp>
#include <NotificationManager.h>
#include "Drawer.h"

namespace Input {
	using json = nlohmann::json;

	class Action {
	public:
		Action(std::string& str) {
			m_actionString = str;
		}
		~Action() {}

		virtual void launch(sf::Event& e) {
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

	class MouseRectAction : public Action {
	public:
		MouseRectAction(std::string& s, sf::Transformable* t, sf::Vector2f& vec) : Action(s) {
			m_transform = t;
			m_size = vec;
		}
		~MouseRectAction() {}

		virtual void launch(sf::Event& e) {
			if (e.type < 7 || e.type > 13)
				return;
			if (!m_transform) {
				Action::launch(e);
				return;
			}
			
			sf::Vector2f mouse = Drawer::I()->getMousePosition();
			Camera* camera = Drawer::I()->getCamera();
			sf::Vector2f pos = m_transform->getPosition();
			sf::Vector2f scale = m_transform->getScale();
			if (mouse.x >= pos.x + camera->getRelativePosition().x)
				if (mouse.x <= pos.x + (m_size.x * scale.x) + camera->getRelativePosition().x)
					if (mouse.y >= pos.y + camera->getRelativePosition().y)
						if (mouse.y <= pos.y + (m_size.y * scale.y) + camera->getRelativePosition().y)
							Action::launch(e);
		}
		
		void setSize(sf::Vector2f& vec) {
			m_size = vec;
		}


	private: 
		sf::Vector2f m_size;
		sf::Transformable* m_transform;
	};

	
	class Manager : public Singleton<Manager>
	{
		friend class Singleton<Manager>;
	public:
		Manager() { load(); }
		~Manager() {
			for (auto& it : m_events) {
				for (auto& it2 : it.second) {
					delete it2.second;
				}
			}
			m_events.clear();
		}

		void addEventTrigger(sf::Event::EventType type, std::string action) {
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, Action*>());
			m_events.at(type).emplace(-1, new Action(action));
		}

		void addKeyboardTrigger(sf::Event::EventType type, sf::Keyboard::Key key, std::string action) {
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, Action*>());
			m_events.at(type).emplace(key,new Action(action));
		}

		//Trigger only if you click on that area
		void addMouseTrigger(sf::Event::EventType type, sf::Mouse::Button button, std::string action, sf::Transformable* t, sf::Vector2f vec) {
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, Action*>());
			m_events.at(type).emplace(button, new MouseRectAction(action, t, vec));
		}

		//Trigger anywhere you click
		void addMouseTrigger(sf::Event::EventType type, sf::Mouse::Button button, std::string action) {
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, Action*>());
			m_events.at(type).emplace(button, new Action(action));
		}

		void removeEventTrigger(sf::Event::EventType type) {
			if (m_events.find(type) == m_events.end())
				return;
			m_events.at(type).erase(-1);
		}

		void removeKeyboardTrigger(sf::Event::EventType type, sf::Keyboard::Key key) {
			if (m_events.find(type) == m_events.end())
				return;
			m_events.at(type).erase(key);
		}

		void removeMouseTrigger(sf::Event::EventType type, sf::Mouse::Button button) {
			if (m_events.find(type) == m_events.end())
				return;
			m_events.at(type).erase(button);
		}


		void handleEvent(sf::Event& event) {
			try {
				if (event.type == 5 || event.type == 6)
					m_events.at(event.type).at(event.key.code)->launch(event);
				else if (event.type >= 7 && event.type <= 13)
					m_events.at(event.type).at(event.mouseButton.button)->launch(event);
				else if (event.type >= 14 && event.type <= 22)
					m_events.at(event.type).at(event.joystickButton.button)->launch(event);
				else
					m_events.at(event.type).at(-1)->launch(event);
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
						m_events.emplace(type, std::map<int, Action*>());
				
					m_events.at(type).emplace(input.at("key").get<int>(), new Action(action));
				}
			}
		}

		
	private:
		std::map<sf::Event::EventType, std::map<int, Action*>>		m_events;
	};
}

#endif

