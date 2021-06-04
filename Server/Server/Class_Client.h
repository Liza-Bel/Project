#ifndef CLASS_CLIENT_H
#define CLASS_CLIENT_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <thread>

enum string_code {
	Delete,
	Create,
	Add,
	Exit,
	History,
	Listrooms,
	Listclients
};
class Clients;

class CallbackInterface {
public:
	virtual void Delete_Room(Clients* cl) = 0;
	virtual void Add_Client_in_the_Room(Clients* cl) = 0;
	virtual void Create_New_Room(std::string name) = 0;
	virtual void Leave_the_Room(Clients* cl) = 0;
	virtual bool Check_name(std::string user_name) = 0;
	virtual std::string list_of_room() = 0;
	virtual std::string list_of_participants_in_the_room(Clients* cl) = 0;
	virtual void Send_message(Clients * cl, char *msg, int msg_size) = 0;
};

class Clients {
public:
	Clients(std::string name, std::string room_name, SOCKET newconnection) : user_name(name), room_name(room_name), connection(newconnection) {}
	std::string get_room();
	std::string get_user_name();
	SOCKET get_connection();
	void change_room(std::string names);
	void connectCallback(CallbackInterface * cb);
	void Parse();
	//static DWORD WINAPI Parse(LPVOID param);
private:
	std::string user_name;
	std::string room_name;
	SOCKET connection;

	CallbackInterface * m_cb;

};

#endif
