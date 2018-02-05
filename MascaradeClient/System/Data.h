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


using json = nlohmann::json;

class Data : public Singleton<Data>
{
	friend class Singleton < Data > ;
public:
	
	json& getJson() { return data; };

	static json& json() { return Data::I()->getJson(); }
	
	static bool exist(std::string& str) { return (Data::json().find(str) != Data::json().end()); }
	static bool exist(const char* str) { return (Data::json().find(str) != Data::json().end()); }

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