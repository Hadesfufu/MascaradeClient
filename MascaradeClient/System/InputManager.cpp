
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include "Log.h"
#include "debug.h"
#include "SFML\System.hpp"
#include <future>
#include <functional>
#include "InputManager.h"
#include "NotificationManager.h"
#include "Parameters.h"
#include <winuser.h>
#include "debug.h"

#define INTERVAL_CLICKING_SECOND			.33f

#define INPUTMAP_PARAMETER_PATH_STR			"InputMap"
#define INPUTMAP_DEFAULT_JOYSTICK_STR		"inputDefaultJoystick"
#define INPUTMAP_DEFAULT_KEY_STR			"inputDefaultKeyboardKey"

namespace Input{
	Manager::Manager() :
		m_usingJoystick(false)
	{
		if (loadInput()){
			Log::info("InputManager") << "-------	InputMap loaded		------\n";
		}
		else{
			Log::error("InputManager") << "-------	InputMap loading error		------\n";
			GfxDbgAssert(false, "error while loading InputMap");
		}
	}
	Manager::~Manager()
	{
		const std::string *inputMapPathPtr = Parameters::I()->getString(INPUTMAP_PARAMETER_PATH_STR);
		if (!inputMapPathPtr){
			Log::error("InputManager") << "can not find inputMap path in Parameters file, save unabled\n";
		}

		m_inputFile.save_file(inputMapPathPtr->c_str());
	}

	bool Manager::handelInputEvent(sf::Event windowEvent){
		switch (windowEvent.type){
		case sf::Event::Closed:
			NotificationManager::I()->PostNotification(NOTIFICATION_NAME_CLOSING_EVENT);
			break;
		case sf::Event::Resized:{
			NotificationManager::I()->PostNotification(NOTIFICATION_NAME_RESIZE_EVENT);
			break;
		}
		case sf::Event::LostFocus:{
			break;
		}
		case sf::Event::GainedFocus:{
			break;
		}
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
			return handleKeyboardEvent(windowEvent);
			break;
		case sf::Event::MouseWheelMoved:
		case sf::Event::MouseWheelScrolled:
			break;
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
		case sf::Event::MouseEntered:
		case sf::Event::MouseLeft:
		case sf::Event::MouseMoved:
			return handleMouseEvent(windowEvent);
			break;
		case sf::Event::JoystickButtonPressed:
		case sf::Event::JoystickButtonReleased:
		case sf::Event::JoystickConnected:
		case sf::Event::JoystickDisconnected:
		case sf::Event::JoystickMoved:
			break;
		case sf::Event::TextEntered:
			//	useless event
				break;
		default:
			Log::warning("InputMaanger") << "error invalide event to handle";
			return false;
			break;
		}

		return true;
	}

	bool Manager::notifyClickingAction(){
		for (auto it = m_pressedCtimeForActions.begin(); it != m_pressedCtimeForActions.end(); it++){
			TimerForKey timer = it->second;
			sf::Time clickingTime = timer.m_clock.getElapsedTime();
			if (clickingTime.asSeconds() >= INTERVAL_CLICKING_SECOND){
				timer.m_clock.restart();
				timer.m_duration += clickingTime;
				NotificationManager::Dictionary userInfo;
				userInfo.m_dict["timeClicking"] = &timer.m_duration;
				NotificationManager::I()->PostNotification(it->first, userInfo);
				it->second = timer;
			}
		}

		return true;
	}

	const std::list<Key> Manager::getKeyForAction(char *actionEventStr) const{
		return m_keyboardActions.keyForSecondKey(actionEventStr);
	}

	bool Manager::resetKeyForEvent(char *actionEventStr, Key currentKey){
		bool linked = m_keyboardActions.unlink(currentKey, actionEventStr);
		if (!linked){
			Log::error("InputManager") << "can not reset key, wrong old kay to unbind set";
			return false;
		}

		return loadDefaultKeyForAction(actionEventStr, m_usingJoystick);
	}
	bool Manager::setKeyForEvent(char *actionEventStr){

		return true;
	}

	bool Manager::loadDefaultKeyForAction(std::string actionStr, bool joystick){
		auto elem = m_defaultKey.find(actionStr);
		if (elem == m_defaultKey.end()){
			Log::error("inputManager") << "error : no default key for action " << actionStr;
			return false;
		}

		auto inputs = elem->second;
		std::map<std::string, Key>::iterator input;
		if (joystick){
			input = inputs.find(INPUTMAP_DEFAULT_JOYSTICK_STR);
		}
		else{
			char keyboardLayoutName[KL_NAMELENGTH];
			keyboardLayoutName[0] = 0;
			GetKeyboardLayoutName(charToLPWSTR(keyboardLayoutName));

			input = inputs.find(keyboardLayoutName);
			if (input == inputs.end())
				input = inputs.find(INPUTMAP_DEFAULT_KEY_STR);
		}

		GfxDbgAssert(input != inputs.end());

		return m_keyboardActions.link(input->second, actionStr);
	}

	bool Manager::loadInput(){
		const std::string *inputMapPathPtr = Parameters::I()->getString(INPUTMAP_PARAMETER_PATH_STR);
		if (!inputMapPathPtr){
			Log::error("InputManager") << "can not find inputMap path in Parameters file";
			return false;
		}
		
		m_inputFile.load_file(inputMapPathPtr->c_str());
		pugi::xml_node inputs = m_inputFile.child("map");
		pugi::xml_node input = inputs.first_child();
		while (input){
			loadAction(input);
			input = input.next_sibling();
		}

		return true;
	}
	void Manager::loadAction(pugi::xml_node actionXml){
		std::string action = actionXml.attribute("action").as_string();
		auto defaultActions = m_defaultKey.find(action);
		if (defaultActions == m_defaultKey.end()){
			defaultActions = m_defaultKey.emplace(action, std::map<std::string, Key>()).first;
		}

		pugi::xml_node localisation = actionXml.child("keyboard");
		while (localisation){
			std::string layout = localisation.attribute("language").as_string();

			Key linkedKey = keyboardKey(localisation);
			auto paired = std::pair<std::string, Key>(layout, linkedKey);
			defaultActions->second.insert(paired);

			localisation = localisation.next_sibling("keyboard");
		}

		pugi::xml_node defaultInput = actionXml.child("default");
		std::string type = defaultInput.attribute("type").as_string();
		if (type == "mouse") {
			auto pairedKey = std::pair<std::string, Key>(INPUTMAP_DEFAULT_KEY_STR, mouseKey(defaultInput));
			defaultActions->second.insert(pairedKey);
		}
		else if (type == "keyboard") {
			auto pairedKey = std::pair<std::string, Key>(INPUTMAP_DEFAULT_KEY_STR, keyboardKey(defaultInput));
			defaultActions->second.insert(pairedKey);
		}
		else
			GfxDbgAssert(false, "unexpected type for input");

		pugi::xml_node defaultJoystick = actionXml.child("defaultJoystick");
		std::string codeJoystick = defaultJoystick.attribute("code").as_string();
		auto pairedKeyJoystick = std::pair<std::string, Key>(INPUTMAP_DEFAULT_JOYSTICK_STR, joystickKey(defaultJoystick));
		defaultActions->second.insert(pairedKeyJoystick);

		pugi::xml_node userDefault = actionXml.child("user");
		if (userDefault) {
			std::string type = userDefault.attribute("type").as_string();
			if (type == "mouse") {
				Key key = mouseKey(userDefault);
				m_keyboardActions.link(key, action);
			}
			else if (type == "keyboard") {
				Key key = keyboardKey(userDefault);
				m_keyboardActions.link(key, action);
			}
			else
				GfxDbgAssert(false, "unexpected type for input");
		}
		else
			loadDefaultKeyForAction(action, false);

		pugi::xml_node userJoystick = actionXml.child("userJoystick");
		if (userJoystick){
			Key userKey = joystickKey(userJoystick);
			m_keyboardActions.link(userKey, action);
		}
		else 
			loadDefaultKeyForAction(action, true);
	}
	Key Manager::keyboardKey(pugi::xml_node keyXml) {
		std::string keyCode = keyXml.attribute("key").as_string();
		std::string status = keyXml.attribute("status").as_string();
		std::string isAlt = keyXml.attribute("alt").as_string();
		std::string isShift = keyXml.attribute("shift").as_string();
		std::string isCtrl = keyXml.attribute("ctrl").as_string();
		std::string isWindow = keyXml.attribute("window").as_string();

		return Key::KeyForKeyboard((sf::Keyboard::Key)std::stoi(keyCode),
			statusForKey(status),
			std::stoi(isAlt) == 1,
			std::stoi(isShift) == 1,
			std::stoi(isCtrl) == 1,
			std::stoi(isWindow) == 1);
	}
	Key Manager::mouseKey(pugi::xml_node keyXml) {
		std::string keyCode = keyXml.attribute("key").as_string();
		std::string status = keyXml.attribute("status").as_string();

		return Key::KeyForMouse(statusForKey(status),
			(sf::Mouse::Button)std::stoi(keyCode));
	}
	Key Manager::joystickKey(pugi::xml_node keyXml) {
		return Key::ErrorKey();
	}
	unsigned int Manager::statusForKey(std::string status) {
		if (status == "pressed")
			return EKeyStatus_Pressed;
		else if (status == "pressing")
			return EKeyStatus_Pressing;
		else if (status == "released")
			return EKeyStatus_Release;
		else if (status == "pressedAndPressing")
			return EKeyStatus_Pressed | EKeyStatus_Pressing;
		else if (status == "pressedAndRelease")
			return EKeyStatus_Pressed | EKeyStatus_Release;
		else if (status == "pressingAndRelease")
			return EKeyStatus_Pressing | EKeyStatus_Release;
		else if (status == "all")
			return EkeyStatus_Any;
		return EKeyStatus_None;
	}

	bool Manager::handleKeyboardEvent(sf::Event &keyboardEvent){
		switch (keyboardEvent.type){
		case sf::Event::KeyPressed:{
			Key key = Key::KeyForKeyboard(keyboardEvent.key.code, Input::EKeyStatus_Pressed, keyboardEvent.key.alt, keyboardEvent.key.shift, keyboardEvent.key.control, keyboardEvent.key.system);
			std::list<std::string> actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				std::string actionName = *it;
				auto it2 = m_pressedCtimeForActions.find(actionName);
				if (it2 == m_pressedCtimeForActions.end())
					NotificationManager::I()->PostNotification(*it);
			}

			key = Key::KeyForKeyboard(keyboardEvent.key.code, Input::EKeyStatus_Pressing, keyboardEvent.key.alt, keyboardEvent.key.shift, keyboardEvent.key.control, keyboardEvent.key.system);
			actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				std::string actionName = *it;
				auto it2 = m_pressedCtimeForActions.find(actionName);
				if (it2 == m_pressedCtimeForActions.end()){
					TimerForKey timer;
					timer.m_clock = sf::Clock();
					timer.m_duration = sf::Time::Zero;
					auto paired = std::pair<std::string, TimerForKey>(*it, timer);
					m_pressedCtimeForActions.insert(paired);
				}
			}

			return true;
		}
		case sf::Event::KeyReleased:{
			Key key = Key::KeyForKeyboard(keyboardEvent.key.code, Input::EKeyStatus_Pressing, keyboardEvent.key.alt, keyboardEvent.key.shift, keyboardEvent.key.control, keyboardEvent.key.system);
			std::list<std::string> actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				auto it2 = m_pressedCtimeForActions.find(*it);
				if (it2 != m_pressedCtimeForActions.end())
					m_pressedCtimeForActions.erase(it2);
			}

			key = Key::KeyForKeyboard(keyboardEvent.key.code, Input::EKeyStatus_Release, keyboardEvent.key.alt, keyboardEvent.key.shift, keyboardEvent.key.control, keyboardEvent.key.system);
			actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				std::string actionName = *it;
				auto it2 = m_pressedCtimeForActions.find(actionName);
				if (it2 == m_pressedCtimeForActions.end())
					NotificationManager::I()->PostNotification(*it);
			}

			return true;
		}
		default:
			return false;
		}

		return true;
	}
	bool Manager::handleMouseEvent(sf::Event &mouseEvent){
		switch (mouseEvent.type){
		case sf::Event::MouseButtonPressed:{
			Key key = Key::KeyForMouse(Input::EKeyStatus_Pressed, mouseEvent.mouseButton.button);
			std::list<std::string> actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				std::string actionName = *it;
				auto it2 = m_pressedCtimeForActions.find(actionName);
				if (it2 == m_pressedCtimeForActions.end())
					NotificationManager::I()->PostNotification(actionName);
			}

			key = Key::KeyForMouse(Input::EKeyStatus_Pressing, mouseEvent.mouseButton.button);
			actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				std::string actionName = *it;
				auto it2 = m_pressedCtimeForActions.find(actionName);
				if (it2 == m_pressedCtimeForActions.end()){
					TimerForKey timer;
					timer.m_clock = sf::Clock();
					timer.m_duration = sf::Time::Zero;
					auto paired = std::pair<std::string, TimerForKey>(*it, timer);
					m_pressedCtimeForActions.insert(paired);
				}
			}

			return true;
		}
		case sf::Event::MouseButtonReleased:{
			Key key = Key::KeyForMouse(Input::EKeyStatus_Pressing, mouseEvent.mouseButton.button);
			std::list<std::string> actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				auto it2 = m_pressedCtimeForActions.find(*it);
				if (it2 != m_pressedCtimeForActions.end())
					m_pressedCtimeForActions.erase(it2);
			}

			key = Key::KeyForMouse(Input::EKeyStatus_Release, mouseEvent.mouseButton.button);
			actions = m_keyboardActions.keyForFirstKey(key);
			for (auto it = actions.begin(); it != actions.end(); it++){
				std::string actionName = *it;
				auto it2 = m_pressedCtimeForActions.find(actionName);
				if (it2 == m_pressedCtimeForActions.end())
					NotificationManager::I()->PostNotification(actionName);
			}

			return true;
		}
		case sf::Event::MouseEntered:
			break;
		case sf::Event::MouseLeft:
			break;
		case sf::Event::MouseMoved:{
			m_mousePos.x = mouseEvent.mouseMove.x;
			m_mousePos.y = mouseEvent.mouseMove.y;
			NotificationManager::Dictionary userInfo;
			userInfo.m_dict[NOTIFICATION_MOUSE_POSITION] = &m_mousePos;
			NotificationManager::I()->PostNotification(NOTIFICATION_NAME_MOUSE_MOVE_EVENT, userInfo);
			return true;
		}
		default:
			return false;
		}

		return true;
	}
}