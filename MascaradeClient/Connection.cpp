#include "Connection.h"
#include "Log.h"

Connection::Connection()
{
}


Connection::~Connection()
{
}

void Connection::connect(std::string address, int port)
{
	sf::Socket::Status status = m_socket.connect(address, port);
	if (status != sf::Socket::Done)
	{
		Log::error("Connection") << "Error while connecting : " << status;
	}
	m_socket.setBlocking(false);
}

void Connection::send(sf::Packet& packet)
{
	if (m_socket.send(packet) != sf::Socket::Done)
	{
		Log::error("Connection-send-string") << "Error while sending to client";
	}
}

sf::Packet* Connection::receive()
{
	if (m_socket.receive(m_lastReceivedPacket) != sf::Socket::Done)
	{
		Log::error("MascaradeServer") << "Error while receiving";
	}
	return &m_lastReceivedPacket;
}


void Connection::disconnect()
{
	m_socket.disconnect();
}


void Connection::appendString(std::string& s, char* c, int nb)
{
	for (int i = 0; i < nb; i++)
	{
		s += c[i];
	}
}