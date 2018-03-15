#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "Singleton.h"
#include <map>
#include "Data.h"
#include "json.hpp"
#include <SFML/Graphics.hpp>
#include <NotificationManager.h>
#include "Drawer.h"
#include "Sprite3D.h"

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
			dico.m_dict.emplace("event",&e);
			NotificationManager::I()->PostNotification(m_actionString, dico);
		}

		NotificationManager::Dictionary& getDictionnary() { return dico; }
	private:
		std::string m_actionString;
		bool m_enabeled = true;
		NotificationManager::Dictionary dico;
	};

	class MouseRectAction : public Action {
	public:
		MouseRectAction(std::string& s, sf::Sprite* sprite) : Action(s) {
			m_sprite = sprite;
		}
		MouseRectAction(std::string& s, sf::Text* text) : Action(s) {
			m_text = text;
		}
		MouseRectAction(std::string& s, sf::Shape* shape) : Action(s) {
			m_shape = shape;
		}
		MouseRectAction(std::string& s, sf::Sprite3d* sprite) : Action(s) {
			m_3dsprite = sprite;
		}
		~MouseRectAction() {}

		virtual void launch(sf::Event& e) {
			if (e.type < 7 || e.type > 13)
				return;
		
			sf::Vector2f mouse = Drawer::I()->getMousePosition();
			Camera* camera = Drawer::I()->getCamera();
			if (m_sprite){
				if (m_sprite->getGlobalBounds().contains(mouse))
					Action::launch(e);
			}
			else if (m_text) {
				if (m_text->getGlobalBounds().contains(mouse))
					Action::launch(e);
			}
			else if (m_shape) {
				if (m_shape->getGlobalBounds().contains(mouse))
					Action::launch(e);
			}
			else if (m_3dsprite) {
				if (m_3dsprite->getGlobalBounds().contains(mouse))
					Action::launch(e);
			}
		
			//Log::debug() << "" << m_3dsprite->getGlobalBounds();
		}


	private: 
		sf::Sprite* m_sprite;
		sf::Text*	m_text;
		sf::Shape*	m_shape;
		sf::Sprite3d* m_3dsprite;
	};

	
	class Manager : public Singleton<Manager>
	{
		friend class Singleton<Manager>;
	public:
		Manager() { load(); }
		~Manager() {
			for (auto& it : m_events) {
				for (auto& it2 : it.second) {
					for (auto& it3 : it2.second) {
						delete it3;
					}
				}
			}
			m_events.clear();
		}

		NotificationManager::Dictionary& addEventTrigger(sf::Event::EventType type, std::string a) {
			Action* action = new Action(a);
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, std::vector<Action*>>());
			m_events.at(type).emplace(-1, std::vector<Action*>());
			m_events.at(type).at(-1).emplace_back(action);
			return action->getDictionnary();			
		}

		NotificationManager::Dictionary& addKeyboardTrigger(sf::Event::EventType type, sf::Keyboard::Key key, std::string a) {
			Action* action = new Action(a);
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, std::vector<Action*>>());
			m_events.at(type).emplace(key, std::vector<Action*>());
			m_events.at(type).at(key).emplace_back(action);
			return action->getDictionnary();
		}

		//Trigger only if you click on that area
		NotificationManager::Dictionary& addMouseTrigger(sf::Event::EventType type, sf::Mouse::Button button, std::string a, sf::Sprite* s) {
			MouseRectAction* action = new MouseRectAction(a, s);
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, std::vector<Action*>>());
			m_events.at(type).emplace(button, std::vector<Action*>());
			m_events.at(type).at(button).emplace_back(action);
			return action->getDictionnary();
		}

		NotificationManager::Dictionary& addMouseTrigger(sf::Event::EventType type, sf::Mouse::Button button, std::string a, sf::Text* t) {
			MouseRectAction* action = new MouseRectAction(a, t);
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, std::vector<Action*>>());
			m_events.at(type).emplace(button, std::vector<Action*>());
			m_events.at(type).at(button).emplace_back(action);
			return action->getDictionnary();
		}

		NotificationManager::Dictionary& addMouseTrigger(sf::Event::EventType type, sf::Mouse::Button button, std::string a, sf::Shape* s) {
			MouseRectAction* action = new MouseRectAction(a, s);
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, std::vector<Action*>>());
			m_events.at(type).emplace(button, std::vector<Action*>());
			m_events.at(type).at(button).emplace_back(action);
			return action->getDictionnary();
		}

		NotificationManager::Dictionary& addMouseTrigger(sf::Event::EventType type, sf::Mouse::Button button, std::string a, sf::Sprite3d* s) {
			MouseRectAction* action = new MouseRectAction(a, s);
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, std::vector<Action*>>());
			m_events.at(type).emplace(button, std::vector<Action*>());
			m_events.at(type).at(button).emplace_back(action);
			return action->getDictionnary();
		}

		//Trigger anywhere you click
		NotificationManager::Dictionary& addMouseTrigger(sf::Event::EventType type, sf::Mouse::Button button, std::string a) {
			Action* action = new Action(a);
			if (m_events.find(type) == m_events.end())
				m_events.emplace(type, std::map<int, std::vector<Action*>>());
			m_events.at(type).emplace(button, std::vector<Action*>());
			m_events.at(type).at(button).emplace_back(action);
			return action->getDictionnary();
		}

		void removeEventTrigger(sf::Event::EventType type) {
			if (m_events.find(type) == m_events.end())
				return;
			m_events.at(type).at(-1).clear();
		}

		void removeKeyboardTrigger(sf::Event::EventType type, sf::Keyboard::Key key) {
			if (m_events.find(type) == m_events.end())
				return;
			m_events.at(type).at(key).clear();
		}

		void removeMouseTrigger(sf::Event::EventType type, sf::Mouse::Button button) {
			if (m_events.find(type) == m_events.end())
				return;
			m_events.at(type).at(button).clear();
		}


		void handleEvent(sf::Event& event) {
			try {
				if (event.type == 5 || event.type == 6)
					for (auto& it : m_events.at(event.type).at(event.key.code)) {
						it->launch(event);
					}
				else if (event.type >= 7 && event.type <= 13)
					for (auto& it : m_events.at(event.type).at(event.mouseButton.button)) {
						it->launch(event);
					}
				else if (event.type >= 14 && event.type <= 22)
					for (auto& it : m_events.at(event.type).at(event.joystickButton.button)) {
						it->launch(event);
					}
				else
					for (auto& it : m_events.at(event.type).at(-1)) {
						it->launch(event);
					}
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
						m_events.emplace(type, std::map<int, std::vector<Action*>>());
				
					m_events.at(type).emplace(input.at("key").get<int>(), std::vector<Action*>());
					m_events.at(type).at(input.at("key")).emplace_back(new Action(action));
				}
			}
		}

		
	private:
		std::map<sf::Event::EventType, std::map<int, std::vector<Action*>>>		m_events;
	};
}

#endif

