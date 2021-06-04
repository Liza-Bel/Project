#ifndef CLIENT_H
#define CLIENT_H
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <thread>
class Client
{
public:
	Client() = default;
	~Client() = default;
	void Strart_Client();
	void Receiving_Messages();
	void Send_Message();
private:
	SOCKET Connection;
	SOCKADDR_IN addr;
};
#endif