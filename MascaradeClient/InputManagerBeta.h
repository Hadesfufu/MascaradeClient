#ifndef InputManagerBeta_H
#define InputManagerBeta_H
#include "Singleton.h"
#include <map>
#include "Data.h"
#include "json.hpp"
#include <SFML/Graphics.hpp>

namespace Input {
	using json = nlohmann::json;

	class Key {
	public:
		Key() {}
		Key(sf::Keyboard::Key k) { m_keyboard = k; }
		Key(sf::Mouse::Button b) { m_mouse = b; }
		Key(sf::Event::EventType b) { m_event = b; }

		void setKeyboardKey(sf::Keyboard::Key k) { m_keyboard = k; }
		void setMouseButton(sf::Mouse::Button b) { m_mouse = b; }
		void setEventType(sf::Event::EventType e) { m_event = e; }

		void load(json& j) {
			
		}

		bool compare(sf::Keyboard::Key k) { return (k == m_keyboard); }
		bool compate(sf::Mouse::Button b) { return (b == m_mouse); }
		bool compate(sf::Event::EventType e) { return (e == m_event); }

	private:
		sf::Keyboard::Key m_keyboard = sf::Keyboard::Unknown;
		sf::Mouse::Button m_mouse = sf::Mouse::Button::ButtonCount;
		sf::Event::EventType m_event = sf::Event::Count; 
	};

	class Input
	{
	public:
		Input(json& j) { load(j); };
		~Input();

		virtual void load(nlohmann::json& j)
		{
			m_status = j.at("status");
			m_action = j.at("action");
		};

		Key getKey() { return m_key; }

	private:
		std::string m_status;
		std::string m_action;
		Key			m_key;
		bool enabled = true; 
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
		sf::Event::EventType m_type;
		int language; 
	};

	class MouseInput : public Input
	{
	public: 
		MouseInput(json& j) : Input(j) {};
		~MouseInput();

	private:

	};

	class ManagerBeta : public Singleton<ManagerBeta>
	{
		friend class Singleton<ManagerBeta>;
	public:
		ManagerBeta() { load(); };
		~ManagerBeta() {};

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
					m_events.emplace(Key(input), action);
				}
			}
		}
	private: 
		std::multimap<Key, std::string>					m_events;
	};
}

#endif

