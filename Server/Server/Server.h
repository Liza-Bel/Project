#ifndef SERVER_H
#define SERVER_H
#include "Class_Room.h"

class Server: public CallbackInterface {
public:
	Server() = default;
	~Server() = default;
	void Start_Server();
	void Connect_Client();
	void Close_Server();
	void Add_Client_in_the_Room(Clients* cl); 
	void Create_New_Room(std::string name);
	void Delete_Room(Clients* cl);
	void Leave_the_Room(Clients* cl);
	void Send_message(Clients* cl);
	bool Check_name(std::string user_name);
	std::string list_of_room();
	std::string list_of_participants_in_the_room(Clients* cl);

private:
	std::vector<Clients> clients_connected;
	std::vector<Room> rooms_server;
	std::vector<std::string> unique_name;

	WSAData wsaData;
	SOCKADDR_IN addr;
	int sizeofaddr;
	SOCKET sListen;
};
#endif
