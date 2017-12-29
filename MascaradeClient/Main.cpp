#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Log.h"
#include "Connection.h"

void appendString(std::string& s, char* c, int nb)
{
	for(int i = 0; i < nb; i++)
	{
		s+= c[i];
	}
}
int main()
{
	Connection connection;
	connection.connect("localhost", 53000);
	std::string s;
	while (1) {
		std::cin >> s;
		connection.send(s);
	}
	system("pause");
	return 0;
}