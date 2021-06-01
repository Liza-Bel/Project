#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>

class CallbackInterface
{
public:
	virtual int cbiCallbackFunction(int) = 0;
};

class Clients {
public:
	Clients(std::string name, std::string room_name, SOCKET newconnection) : user_name(name), room_name(room_name), ñonnection(newconnection) {}
	std::string get_room();
	std::string get_user_name();
	void change_room(std::string names);
	void connectCallback(CallbackInterface * cb);
	void Parse();
private:
	std::string user_name;
	std::string room_name;
	SOCKET ñonnection;

	CallbackInterface * m_cb;

};
