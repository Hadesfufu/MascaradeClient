////////////////////////////////////////////////////////////
// < Fichier : "MascaradeClient.h"> 
// < Author : Yohan "HadesFufu" Masson > 
//////////////////////////////////////////////////////////// 
#ifndef MASCARADECLIENT_H
#define MASCARADECLIENT_H
#include "Singleton.h"
#include "Connection.h"

class MascaradeClient : public Singleton<MascaradeClient>
{
	friend class Singleton<MascaradeClient>;
public:

	void launch();
	void login(); 
private:
	MascaradeClient();
	~MascaradeClient();

	Connection m_connection;
	sf::Packet m_packet;
};


#endif