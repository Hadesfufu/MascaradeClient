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
	
	json& getJson() { return m_json; };

	static json& json() { return Data::I()->getJson(); }

private:
	 Data();
	~Data();

	void load(); // load all Data

	////////////////////////////////////////////////////////////
    // Members
    ////////////////////////////////////////////////////////////
	
	nlohmann::json m_json;
	std::fstream m_fileStream;
};
#endif 