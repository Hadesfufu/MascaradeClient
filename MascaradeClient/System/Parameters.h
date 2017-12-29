////////////////////////////////////////////////////////////
// < Fichier : "Parameters.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

//	modify by Vassili "Bakasama" Rezvoy - 06/06/2016

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Log.h"
#include "pugixml.hpp"
#include "Singleton.h"

class Parameters : public Singleton<Parameters>
{
	friend class Singleton < Parameters > ;
public:

	const bool*			getBool(const char* s){
		std::string string = std::string(s);
		auto it = m_datas.find(string);

		if (it != m_datas.end())
			return convert<bool>(it);
		else
			return loadBool(string);
	}

	const int*			getInt		(const char* s){
		std::string string = std::string(s);
		auto it = m_datas.find(string);
		
		if (it != m_datas.end())
			return convert<int>(it);
		else
			return loadInt(string);
	}
	const float*		getFloat	(const char* s){
		std::string string = std::string(s);
		auto it = m_datas.find(string);

		if (it != m_datas.end())
			return convert<float>(it);
		else
			return loadFloat(string);
	}
	const double*		getDouble	(const char* s){
		std::string string = std::string(s);
		auto it = m_datas.find(string);

		if (it != m_datas.end())
			return convert<double>(it);
		else
			return loadDouble(string);
	}
	const unsigned int* getUInt		(const char* s){
		std::string string = std::string(s);
		auto it = m_datas.find(string);

		if (it != m_datas.end())
			return convert<unsigned int>(it);
		else
			return loadUInt(string);
	}
	const sf::Vector2i* getVec2I	(const char* s){
		std::string string = std::string(s);
		auto it = m_datas.find(string);

		if (it != m_datas.end())
			return convert<sf::Vector2i>(it);
		else
			return loadVec2I(string);
	}
	const sf::Vector2f* getVec2F	(const char* s){
		std::string string = std::string(s);
		auto it = m_datas.find(string);

		if (it != m_datas.end())
			return convert<sf::Vector2f>(it);
		else
			return loadVec2F(string);
	}
	const sf::Vector2u*	getVec2U	(const char* s){
		std::string string = std::string(s);
		auto it = m_datas.find(string);

		if (it != m_datas.end())
			return convert<sf::Vector2u>(it);
		else
			return loadVec2U(string);
	}
	const std::string*	getString	(const char* s){
		std::string string = std::string(s);
		auto it = m_datas.find(string);

		if (it != m_datas.end())
			return convert<std::string>(it);
		else
			return loadString(string);
	}

	void save();

	void set(char* paramName, bool value);
	void set(char* paramName, int value);
	void set(char* paramName, float value);
	void set(char* paramName, unsigned int value);
	void set(char* paramName, double value);
	void set(char* paramName, sf::Vector2f value);
	void set(char* paramName, sf::Vector2i value);
	void set(char* paramName, sf::Vector2u value);

private:
	 Parameters();
	~Parameters();

	void load(); // load all parameters

	template<class T>
	const T* convert(std::map< std::string, std::shared_ptr<void>>::iterator& it){
		T* result;
		try{
			result = static_cast<T*>(it->second.get());
		}
		catch (...){
			Log::error("Parameters") << "Error on convertion";
			return nullptr;
		};
		return result;
	}
	
	const bool*			loadBool(std::string& s){
		sf::Lock locker(m_locker);

		pugi::xml_node elem = m_root.child(s.c_str());
		if (!elem)
			return nullptr;

		std::shared_ptr<void> ptr = std::make_shared<bool>((bool)elem.first_attribute().as_bool());
		m_datas.insert(m_datas.begin(), std::pair<std::string, std::shared_ptr<void>>(s, ptr));
		return static_cast<bool*>(m_datas[s].get());
	}

	const int*			loadInt		(std::string& s){
		sf::Lock locker(m_locker);

		pugi::xml_node elem = m_root.child(s.c_str());
		if (!elem)
			return nullptr;

		std::shared_ptr<void> ptr = std::make_shared<int>((int)elem.first_attribute().as_int());
		m_datas.insert(m_datas.begin(), std::pair<std::string, std::shared_ptr<void>>(s, ptr));
		return static_cast<int*>(m_datas[s].get());
	}
	const float*		loadFloat	(std::string &s){
		sf::Lock locker(m_locker);

		pugi::xml_node elem = m_root.child(s.c_str());
		if (!elem)
			return nullptr;

		std::shared_ptr<void> ptr = std::make_shared<float>((float)elem.first_attribute().as_float());
		m_datas.insert(m_datas.begin(), std::pair<std::string, std::shared_ptr<void>>(s, ptr));
		return static_cast<float*>(m_datas[s].get());
	}
	const double*		loadDouble	(std::string &s){
		sf::Lock locker(m_locker);

		pugi::xml_node elem = m_root.child(s.c_str());
		if (!elem)
			return nullptr;

		std::shared_ptr<void> ptr = std::make_shared<double>((double)elem.first_attribute().as_double());
		m_datas.insert(m_datas.begin(), std::pair<std::string, std::shared_ptr<void>>(s, ptr));
		return static_cast<double*>(m_datas[s].get());
	}
	const unsigned int* loadUInt	(std::string &s){
		sf::Lock locker(m_locker);

		pugi::xml_node elem = m_root.child(s.c_str());
		if (!elem)
			return nullptr;

		std::shared_ptr<void> ptr = std::make_shared<unsigned int>((unsigned int)elem.first_attribute().as_uint());
		m_datas.insert(m_datas.begin(), std::pair<std::string, std::shared_ptr<void>>(s, ptr));
		return static_cast<unsigned int*>(m_datas[s].get());
	}
	const sf::Vector2i* loadVec2I	(std::string &s){
		sf::Lock locker(m_locker);

		pugi::xml_node elem = m_root.child(s.c_str());
		if (!elem)
			return nullptr;

		std::shared_ptr<void> ptr = std::make_shared<sf::Vector2i>(sf::Vector2i(elem.first_attribute().as_int(), elem.last_attribute().as_int()));
		m_datas.insert(m_datas.begin(), std::pair<std::string, std::shared_ptr<void>>(s, ptr));
		return static_cast<sf::Vector2i*>(m_datas[s].get());
	}
	const sf::Vector2f* loadVec2F	(std::string &s){
		sf::Lock locker(m_locker);

		pugi::xml_node elem = m_root.child(s.c_str());
		if (!elem)
			return nullptr;

		std::shared_ptr<void> ptr = std::make_shared<sf::Vector2f>(sf::Vector2f(elem.first_attribute().as_float(), elem.last_attribute().as_float()));
		m_datas.insert(m_datas.begin(), std::pair<std::string, std::shared_ptr<void>>(s, ptr));
		return static_cast<sf::Vector2f*>(m_datas[s].get());
	}
	const sf::Vector2u* loadVec2U	(std::string &s){
		sf::Lock locker(m_locker);

		pugi::xml_node elem = m_root.child(s.c_str());
		if (!elem)
			return nullptr;

		std::shared_ptr<void> ptr = std::make_shared<sf::Vector2u>(sf::Vector2u(elem.first_attribute().as_uint(), elem.last_attribute().as_uint()));
		m_datas.insert(m_datas.begin(), std::pair<std::string, std::shared_ptr<void>>(s, ptr));
		return static_cast<sf::Vector2u*>(m_datas[s].get());
	}
	const std::string*	loadString	(std::string &s){
		sf::Lock locker(m_locker);

		pugi::xml_node elem = m_root.child(s.c_str());
		if (!elem)
			return nullptr;

		std::shared_ptr<void> ptr = std::make_shared<std::string>((std::string)elem.first_attribute().as_string());
		m_datas.insert(m_datas.begin(), std::pair<std::string, std::shared_ptr<void>>(s, ptr));
		return static_cast<std::string*>(m_datas[s].get());
	}

	////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////
private:
	sf::Mutex										m_locker;

	std::map< std::string, std::shared_ptr<void> >	m_datas;

	pugi::xml_node									m_root;
	pugi::xml_document								m_doc;
};

#endif 
