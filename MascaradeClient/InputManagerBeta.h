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
		~Key() {}
		Key(sf::Keyboard::Key k) { m_keyboard = k; }
		Key(sf::Mouse::Button b) { m_mouse = b; }
		Key(sf::Event::EventType b) { m_event = b; }
		Key(json& j) { load(j); }

		void setKeyboardKey(sf::Keyboard::Key& k) { m_keyboard = k; }
		void setMouseButton(sf::Mouse::Button& b) { m_mouse = b; }
		void setEventType(sf::Event::EventType& e) { m_event = e; }

		void load(json& j) {
			try {
				std::string type = j.at("type");
				if (type == "keyboard") {
					try {
						m_keyboard = sf::Keyboard::Key(j.at("key"));
					}
					catch (...) { Log::error("Key::load") << "keyboard key not found"; }
				}
				else if (type == "keyboard") {
					try {
						m_mouse = sf::Mouse::Button(j.at("key"));
					}
					catch (...) { Log::error("Key::load") << "mouse key not found"; }
				}
				else if (type == "event") {
					try {
						m_event = sf::Event::EventType(j.at("key"));
					}
					catch (...) { Log::error("Key::load") << "event key not found"; }
				}
			}
			catch (...) {}
		}

		bool compare(sf::Keyboard::Key k) const { return (k == m_keyboard); }
		bool compate(sf::Mouse::Button b) const { return (b == m_mouse); }
		bool compate(sf::Event::EventType e) const { return (e == m_event); }

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
			m_status = j.at("status").get<std::string>();
			m_action = j.at("action").get<std::string>();
			m_key.load(j);
		};

		Key getKey() { return m_key; }

		void enable(bool b) { m_enabled = b; }

		bool checkInput(sf::Event e) {

		}

	private:
		std::string				m_action;
		sf::Event::EventType	m_status; 
		Key						m_key;
		bool					m_enabled = true;
	};

	class KeyboardInput : public Input
	{
	public:
		KeyboardInput(json& j) : Input(j) {};
		~KeyboardInput();

		void load(nlohmann::json& j)
		{
			Input::load(j);
			m_alt = j.at("alt");
			m_shift = j.at("shift");
			m_ctrl = j.at("ctrl");
			m_windows = j.at("windows");
		}

	private:
		bool m_alt, m_shift, m_ctrl, m_windows;
		int language;
	};

	class MouseInput : public Input
	{
	public:
		MouseInput(json& j) : Input(j) {}
		~MouseInput() {}

		void load(json& j) {
			Input::load(j);
		}

	private:
		
	};

	class ManagerBeta : public Singleton<ManagerBeta>
	{
		friend class Singleton<ManagerBeta>;
	public:
		ManagerBeta() { load(); }
		~ManagerBeta() {
			for (auto it = m_events.begin(); it != m_events.end(); ++it) {
				delete (*it).first;
			}
			m_events.clear();
		}

		void addEventTrigger(sf::Event::EventType type, std::string& string) {
			m_events.emplace(new Key(type), string);
		}

		void handleEvent(sf::Event& event) {
			for (auto it = m_events.begin(); it != m_events.end(); ++it) {
				if ((*it).first->compate(event.type)) {

				}
				else if (event.type == sf::Event::KeyPressed) {

				}
			}
		}

		void load()
		{
			std::string type;
			std::string action;
			Input* inputptr;
			for (auto& actions : Data::json().at("InputMap"))
			{
				action = actions.at("action").get<std::string>();
				for (auto& input : actions.at("inputs"))
				{
					type = actions.at("type").get<std::string>();
					if (type == "keyboard") {
						inputptr = new KeyboardInput(input);
					}
					else if (type == "mouse") {
						inputptr = new MouseInput(input);
					}
					m_events.emplace(new Key(input), inputptr);
				}
			}
		}
	private:
		std::multimap<Key*, Input*>					m_events;
	};
}

#endif

