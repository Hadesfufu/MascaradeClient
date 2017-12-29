#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
////////////////////////////////////////////////////////////
//
//-------------------BY HadesFufu---------------------
// >French<
//Utilisation : Permet d'avoir acces a une unique instance de la classe. 
//
//Fonctionnement : stocke un pointeur vers sa propre classe. 
//
////////////////////////////////////////////////////////////

template<class T>
class Singleton
{
public:
	////////////////////////////
	// function : if the instance doesn't exist : it creates it.
	// @return : return current instance.
	////////////////////////////
	static T*		getInstance(){
		static T m_instance;
		return &m_instance;
	};
	static T*		I(){
		return getInstance();
	}

protected:
	////////////////////////////
	// Constructor
	////////////////////////////
	Singleton()
	{
	
	};

	////////////////////////////
	// Destructor
	////////////////////////////
	virtual ~Singleton()
	{

	};
};

#endif
