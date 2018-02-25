////////////////////////////////////////////////////////////
// < Fichier : "Data.h"> 
// < Author : Yohan "HadesFufu" Masson >
////////////////////////////////////////////////////////////

#ifndef Data_H
#define Data_H


#include <map>
#include <memory>
#include "Log.h"
#include "json.hpp"
#include "Singleton.h"
#include <fstream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Utf.hpp>
#include <SFML/System/String.hpp>


using json = nlohmann::json;

class Data : public Singleton<Data>
{
	friend class Singleton < Data > ;
public:
	
	json& getJson() { return data; };

	static json& json() { return Data::I()->getJson(); }
	
	static bool exist(nlohmann::json& j, std::string& str) { return (j.find(str) != j.end()); }
	static bool exist(nlohmann::json& j, const char* str) { return (j.find(str) != j.end()); }
	

	////////////////////////////////////////////////////////////
	// Use This for Custom class
	////////////////////////////////////////////////////////////
	template<typename  T> static T get(nlohmann::json& j) { return j;  }

	template<> static sf::Color get<sf::Color>(nlohmann::json& j){
		if (exist(j, "rgba"))
			return sf::Color(j.get<sf::Uint32>());
		else
			return sf::Color(j.at("r"), j.at("g"), j.at("b"), j.at("a"));
	}

	template<> static sf::Vector2f get<sf::Vector2f>(nlohmann::json& j) { return sf::Vector2f(j.at("x"), j.at("y")); }
	template<> static sf::Vector2i get<sf::Vector2i>(nlohmann::json& j) { return sf::Vector2i(j.at("x"), j.at("y")); }
	template<> static sf::Vector2u get<sf::Vector2u>(nlohmann::json& j) { return sf::Vector2u(j.at("x"), j.at("y")); }
	template<> static sf::String get<sf::String>(nlohmann::json& j) { return sf::String(j.get<std::string>()); }

	nlohmann::json data;

private:
	 Data();
	~Data();

	void load(); // load all Data

	////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////
	
	std::fstream m_fileStream;
};
#endif 