////////////////////////////////////////////////////////////
// < Fichier : "Connection.h"> 
// < Author : Yohan "HadesFufu" Masson > 
//////////////////////////////////////////////////////////// 
#ifndef CONNECTION_H
#define CONNECTION_H
#include "Singleton.h"
#include <SFML/Network.hpp>

class Connection 
{

public:
	Connection();
	~Connection();

	void connect(std::string address, int port);
	void disconnect();

	void send(sf::Packet&);
	sf::Packet* receive();

	int getRemotePort() { return m_socket.getRemotePort(); }
	int getLocalPort() { return m_socket.getLocalPort(); }
	std::string getAddress() { return m_socket.getRemoteAddress().toString(); }

	sf::Packet* getPacket() { return &m_lastReceivedPacket; }
private:

	void appendString(std::string& s, char* c, int nb);
	sf::TcpSocket m_socket;
	sf::Packet					m_lastReceivedPacket;

};

#endif