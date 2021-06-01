#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>

class Client
{
public:
	Client() = default;
	~Client() = default;
	void Strart_Client();
private:
	SOCKET Connection;
	SOCKADDR_IN addr;
};