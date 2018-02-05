#ifndef InputManager_H
#define InputManager_H

#include <SFML/Graphics.hpp>
#include <map>
#include <list>
#include "Singleton.h"
#include "InputKey.h"
#include "reversableHashTable.h"
#include "json.hpp"

#define WINDOW_MODE_WINDOWED				0
#define WINDOW_MODE_FULLSCREEN				1
#define WINDOW_MODE_BORDERLESS				2
#define WINDOW_MODE_FULLSCREEN_BORDERLESS	3

#define NOTIFICATION_NAME_CLOSING_EVENT		"notificationNameClosingEvent"
#define NOTIFICATION_NAME_RESIZE_EVENT		"notificationNameResizeEvent"
#define NOTIFICATION_NAME_FOCUS_EVENT		"notificationNameFocusEvent"

#define NOTIFICATION_NAME_MOUSE_MOVE_EVENT	"notificationNameMouseMoveEvent"
#define NOTIFICATION_MOUSE_POSITION			"notificationMousePosition"

namespace Input{
	class Manager : public Singleton<Manager>
	{
		friend class Singleton<Manager>;
	public:
		typedef struct {
			bool		m_isPressed;
			sf::Time	m_timePressed;
		} PressedData;

	public:
		/*
		main function, call it when pulling event from window to be handle by the input manager
		it will notify register object for the appropriate action
		*/
		bool							handelInputEvent(sf::Event windowEvent);

		/*
		main funtion, call it after pulling all event
		it will notify object register for clicking action when requiered time is ellapsed
		*/
		bool							notifyClickingAction();

		/*
		return list of key for action
		*/
		const std::list<Key>			getKeyForAction(char *actionName) const;

		/*
		return info about action (if its pressed from a key or not, 
		and in the case it is the time ellasped since first press)
		*/
		PressedData						isActionPressed(char *actionName);

		/*
		use it to reset action according to current keyboard layout
		*/
		bool							resetKeyForEvent(char *action, Key currentKey);

		/*
		use it to set action with next keyEvent detected	
		*/
		bool							setKeyForEvent(char *);

		inline sf::Vector2i				getMousePos()const { return m_mousePos; }

	private:
		typedef struct {
			sf::Clock	m_clock;
			sf::Time	m_duration;
		} TimerForKey;

	private:
		Manager();
		Manager(const Manager&) = delete;
		~Manager();
		void							callClickingActions();

		bool							loadDefaultKeyForAction(std::string, bool);

		bool							loadInput();
//	healpers
		void							loadAction(nlohmann::json& action);
		Key								keyboardKey(nlohmann::json&);
		Key								mouseKey(nlohmann::json&);
		//	todo
		Key								joystickKey(nlohmann::json& );
		unsigned int					statusForKey(std::string);

		bool							handleKeyboardEvent(sf::Event &);
		bool							handleMouseEvent(sf::Event &);

	private:
		bool												m_usingJoystick;

		ReversableHashTable<Key, std::string>				m_keyboardActions;
		std::map<std::string, std::map<std::string, Key>>	m_defaultKey;
		std::map<std::string, TimerForKey>					m_pressedCtimeForActions;

		sf::Vector2i										m_mousePos;
	}; 
}

#endif