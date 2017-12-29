
#include "InputKey.h"

namespace Input{
	bool KeyDataKeyboard::operator== (const KeyDataKeyboard data) const{
		if (m_keyStatus != data.m_keyStatus ||
			m_code != data.m_code ||
			m_atlPressed != data.m_atlPressed ||
			m_shiftPressed != data.m_shiftPressed ||
			m_ctrlPressed != data.m_ctrlPressed ||
			m_windowPressed != data.m_windowPressed)
			return false;
		return true;
	}
	bool KeyDataKeyboard::operator<(const KeyDataKeyboard data) const{
		if (m_code == data.m_code){
			if (m_keyStatus == data.m_keyStatus){
				if (m_atlPressed == data.m_atlPressed){
					if (m_ctrlPressed == data.m_ctrlPressed){
						if (m_shiftPressed == data.m_shiftPressed){
							if (m_windowPressed == data.m_windowPressed)
								return false;
							return m_windowPressed < data.m_windowPressed;
						}
						return m_ctrlPressed < data.m_ctrlPressed;
					}
					return m_ctrlPressed < data.m_ctrlPressed;
				}
				return m_atlPressed < data.m_atlPressed;
			}
			return m_keyStatus < data.m_keyStatus;
		}
		return m_code < data.m_code;
	}

	bool KeyDataMouse::operator== (const KeyDataMouse data) const{
		if (m_keyStatus != data.m_keyStatus ||
			m_button != data.m_button)
			return false;
		return true;
	}
	bool KeyDataMouse::operator<(const KeyDataMouse data) const{
		if (m_button == data.m_button)
			return m_keyStatus < data.m_keyStatus;

		return m_button < data.m_button;
	}

	bool KeyDataScrolling::operator== (const KeyDataScrolling data) const{
		return false;
	}
	bool KeyDataScrolling::operator<(const KeyDataScrolling data) const{
		return false;
	}

	bool KeyDataJoystick::operator== (const KeyDataJoystick data) const{
		return false;
	}
	bool KeyDataJoystick::operator<(const KeyDataJoystick data) const{
		return false;
	}

	Key Key::KeyForKeyboard(sf::Keyboard::Key code, unsigned int keyStatus, bool altPressed, bool shiftPRessed, bool ctrlPressed, bool windowPressed){
		KeyDataKeyboard status;
		status.m_code = code;
		status.m_keyStatus = keyStatus;
		status.m_atlPressed = altPressed;
		status.m_shiftPressed = shiftPRessed;
		status.m_ctrlPressed = ctrlPressed;
		status.m_windowPressed = windowPressed;

		return Key(status);
	}
	Key Key::KeyForMouse(unsigned int keyStatus, sf::Mouse::Button button){
		KeyDataMouse status;
		status.m_keyStatus = keyStatus;
		status.m_button = button;

		return Key(status);
	}
	Key Key::KeyForScrolling(){
		KeyDataScrolling status;
		return Key(status);
	}
	Key Key::ErrorKey(){
		Key result;
		result.m_type = EKeyType_Unknow;

		return result;
	}

	bool Key::operator==(const Key a) const{
		if (this->m_type != a.m_type)
			return false;

		switch (a.m_type){
		case EKeyType_Unknow:
			return false;
		case EKeyType_Keyboard:
			return this->m_keyboardKeyData == a.m_keyboardKeyData;
		case EKeyType_Mouse:
			return this->m_mouseKeyData == a.m_mouseKeyData;
		case EKeyType_Scrolling:
			return this->m_scrollingData == a.m_scrollingData;
		case EKeyType_Joystick:
			return this->m_joystickKeyData == a.m_joystickKeyData;
		default:
			return false;
		}
	}
	bool Key::operator<(const Key a) const{
		if (this->m_type < a.m_type)
			return true;
		if (this->m_type > a.m_type)
			return false;

		switch (a.m_type){
		case EKeyType_Unknow:
			return false;
		case EKeyType_Keyboard:
			return this->m_keyboardKeyData < a.m_keyboardKeyData;
		case EKeyType_Mouse:
			return this->m_mouseKeyData < a.m_mouseKeyData;
		case EKeyType_Scrolling:
			return this->m_scrollingData < a.m_scrollingData;
		case EKeyType_Joystick:
			return this->m_joystickKeyData < a.m_joystickKeyData;
		default:
			return false;
		}
	}

	Key::Key() :
		m_type(EKeyType_Unknow)
	{

	}
	Key::Key(KeyDataKeyboard status) :
		m_type(EKeyType_Keyboard),
		m_keyboardKeyData(status)
	{

	}
	Key::Key(KeyDataMouse status) :
		m_type(EKeyType_Mouse),
		m_mouseKeyData(status)
	{

	}
	Key::Key(KeyDataScrolling status) :
		m_type(EKeyType_Scrolling),
		m_scrollingData(status)
	{

	}
	Key::~Key()
	{

	}
}