#include "Connection.h"
#include "Log.h"

const std::string Connection::eom = "eom;";

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
}

void Connection::send(std::string send)
{
	send += eom;
	sf::Socket::Status status = m_socket.send(send.c_str(), send.size());
	if (status != sf::Socket::Done)
	{
		Log::error("Connection") << "Error while sending to client : " << status;
	}
}

std::string Connection::receive()
{
	std::string returner="";
	size_t size = 32;
	char data[32];
	std::size_t received;

	while (returner.find(eom) == std::string::npos) {
		if (m_socket.receive(data, size, received) != sf::Socket::Done)
		{
			Log::error("MascaradeServer") << "Error while receiving";
		}
		appendString(returner, data, received);
		Log::debug() << "new loop";
	}
	returner.erase(returner.end() - 4,returner.end());
	return returner;
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