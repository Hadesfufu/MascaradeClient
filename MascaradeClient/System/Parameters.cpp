
#include "Parameters.h"

Parameters::Parameters()
{
	m_locker.lock();
	load();
	m_locker.unlock();
}
Parameters::~Parameters()
{
	m_doc.save_file("data/param.ini");
	m_datas.clear();
}

void Parameters::load(void){
	std::string path("data/param.ini");
	if (!m_doc.load_file(path.c_str())){
		Log::error("Parameters") << "Unable to open the parameters file";
		return;
	}
	m_root = m_doc.first_child();
}

void Parameters::save(){
	m_doc.save_file("data/param.ini");
}

void Parameters::set(char* paramName, bool value){
	m_locker.lock();

	pugi::xml_node elem = m_root.child(paramName);
	if (elem)
		elem.first_attribute().set_value(value);

	if (getBool(paramName))
		m_datas[paramName] = std::make_shared<bool>(value);

	m_locker.unlock();
}

void Parameters::set(char* paramName, int value){
	m_locker.lock();

	pugi::xml_node elem = m_root.child(paramName);
	if (elem)
		elem.first_attribute().set_value(value);

	if (getInt(paramName))
		m_datas[paramName] = std::make_shared<int>(value);

	m_locker.unlock();
}
void Parameters::set(char* paramName, float value){
	m_locker.lock();

	pugi::xml_node elem = m_root.child(paramName);
	if (elem)
		elem.first_attribute().set_value(value);

	if (getFloat(paramName))
		m_datas[paramName] = std::make_shared<float>(value);

	m_locker.unlock();
}
void Parameters::set(char* paramName, unsigned int value){
	m_locker.lock();

	pugi::xml_node elem = m_root.child(paramName);
	if (elem)
		elem.first_attribute().set_value(value);

	if (getUInt(paramName))
		m_datas[paramName] = std::make_shared<unsigned int>(value);

	m_locker.unlock();
}
void Parameters::set(char* paramName, double value){
	m_locker.lock();

	pugi::xml_node elem = m_root.child(paramName);
	if (elem)
		elem.first_attribute().set_value(value);

	if (getDouble(paramName))
		m_datas[paramName] = std::make_shared<double>(value);

	m_locker.unlock();
}
void Parameters::set(char* paramName, sf::Vector2f value){
	m_locker.lock();

	pugi::xml_node elem = m_root.child(paramName);
	if (elem){
		elem.first_attribute().set_value(value.x);
		elem.last_attribute().set_value(value.y);
	}

	if (getDouble(paramName))
		m_datas[paramName] = std::make_shared<sf::Vector2f>(value);

	m_locker.unlock();
}
void Parameters::set(char* paramName, sf::Vector2i value){
	m_locker.lock();

	pugi::xml_node elem = m_root.child(paramName);
	if (elem){
		elem.first_attribute().set_value(value.x);
		elem.last_attribute().set_value(value.y);
	}

	if (getDouble(paramName))
		m_datas[paramName] = std::make_shared<sf::Vector2i>(value);

	m_locker.unlock();
}
void Parameters::set(char* paramName, sf::Vector2u value){
	m_locker.lock();

	pugi::xml_node elem = m_root.child(paramName);
	if (elem){
		elem.first_attribute().set_value(value.x);
		elem.last_attribute().set_value(value.y);
	}

	if (getDouble(paramName))
		m_datas[paramName] = std::make_shared<sf::Vector2u>(value);

	m_locker.unlock();
}