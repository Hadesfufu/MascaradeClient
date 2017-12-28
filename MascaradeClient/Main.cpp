#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC

#include <SFML/Graphics.hpp>
#include "Log.h"
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")



void appendString(std::string& s, char* data, int nbChar)
{
	for (int i = 0; i < nbChar; i++)
	{
		s += data[i];
	}
}

int main()
{
	/*sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}
	*/

	WSADATA WSAData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	SOCKADDR_IN sin;
	struct addrinfo hints;
	struct addrinfo *result = nullptr;
	struct addrinfo *ptr = nullptr;

	int recvbuflen = 512;
	char recvbuf[512];

	int iResult = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (iResult != 0) {
		Log::error("Main") << "WSAStartup failed with error:" << iResult;
		return 1;
	}

	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


	iResult = getaddrinfo("localhost", "8080", &hints, &result);
	if (iResult != 0) {
		Log::error("Main") << "getaddrinfo failed with error:" << iResult;
		WSACleanup();
		return 1;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			Log::error("Main") << "socket failed with error: %ld\n" << WSAGetLastError();
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	if (ConnectSocket == INVALID_SOCKET) {
		Log::error("Main") << "Unable to connect to server!\n";
		WSACleanup();
		return 1;
	}

	std::string data;
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			Log::info("Main") << "Bytes received:" << iResult;
		else if (iResult == 0)
			Log::info("Main") << "Connection closed\n";
		else
			Log::error("Main") << "recv failed with error:" <<  WSAGetLastError();
		appendString(data, recvbuf, iResult);
	} while (iResult > 0);

	Log::debug() << "all data = " << data;

	freeaddrinfo(result);
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}