#include "MascaradeClient.h"
#include <iostream>
#include "Log.h"


MascaradeClient::MascaradeClient()
{
}


MascaradeClient::~MascaradeClient()
{
}

void MascaradeClient::launch()
{
	m_connection.connect("localhost", 53000);
	login();	
	m_connection.disconnect();
}

void MascaradeClient::login()
{
	Log::debug() << "login begin";
	std::string s;
	std::cin >> s;
	m_packet.clear();
	m_packet << "login()" << "name:"+s;
	m_connection.send(m_packet);
	Log::debug() << "login end";
}

