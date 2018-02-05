#include "Data.h"

Data::Data()
{
	load();
}
Data::~Data()
{
	m_fileStream << data;
	m_fileStream.close();
}

void Data::load(void){
	try {
		m_fileStream.open("data.dat", std::fstream::in | std::fstream::out);
		if (!m_fileStream.is_open())
			Log::error("Data::load") << "file not found";
		m_fileStream >> data;
	}
	catch(std::fstream::failure e)
	{
		Log::debug("Data::load") << "Error while opening the file" << e.what();
	}
}
