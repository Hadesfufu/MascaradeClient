#ifndef INPUT_KEY_H_INCLUDED
#define INPUT_KEY_H_INCLUDED

#include <SFML/Window.hpp>

namespace Input{
	typedef enum {
		EKeyType_Unknow = -1,
		EKeyType_Keyboard = 0,
		EKeyType_Mouse,
		EKeyType_Scrolling,
		EKeyType_Joystick,
		EKeyType_Count
	} EKeyType;
	typedef enum {
		EKeyStatus_None = 0,
		EKeyStatus_Pressed = 1 << 0,
		EKeyStatus_Pressing = 1 << 1,
		EKeyStatus_Release = 1 << 2,
		EkeyStatus_Any = EKeyStatus_Pressed | EKeyStatus_Pressing | EKeyStatus_Release
	} EKeyStatus;
	struct KeyDataKeyboard{
		unsigned int		m_keyStatus;
		sf::Keyboard::Key	m_code;
		bool				m_atlPressed;
		bool				m_shiftPressed;
		bool				m_ctrlPressed;
		bool				m_windowPressed;

		bool operator==(const KeyDataKeyboard data) const;
		bool operator<(const KeyDataKeyboard data) const;
	};
	struct KeyDataMouse{
		unsigned int		m_keyStatus;
		sf::Mouse::Button	m_button;

		bool operator==(const KeyDataMouse) const;
		bool operator<(const KeyDataMouse data) const;
	};
	struct KeyDataScrolling{
		bool operator==(const KeyDataScrolling) const;
		bool operator<(const KeyDataScrolling data) const;
	};
	struct KeyDataJoystick{
		bool operator==(const KeyDataJoystick) const;
		bool operator<(const KeyDataJoystick data) const;
	};

	class Key
	{
	public:
		static Key KeyForKeyboard(sf::Keyboard::Key code, unsigned int keyStatus, bool altPressed, bool shiftPRessed, bool ctrlPressed, bool windowPressed);
		static Key KeyForMouse(unsigned int keyStatus, sf::Mouse::Button	button);
		static Key KeyForScrolling();
		static Key ErrorKey();
		~Key();

		bool operator==(const Key a) const;
		bool operator<(const Key a) const;

	private:
		Key();
		Key(KeyDataKeyboard status);
		Key(KeyDataMouse status);
		Key(KeyDataScrolling status);

	private:
		EKeyType				m_type;
		union{
			KeyDataKeyboard		m_keyboardKeyData;
			KeyDataMouse		m_mouseKeyData;
			KeyDataScrolling	m_scrollingData;
			KeyDataJoystick		m_joystickKeyData;
		};
	};
}

#endif
