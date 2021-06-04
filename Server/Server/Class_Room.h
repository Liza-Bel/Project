#ifndef CLASS_ROOM_H
#define CLASS_ROOM_H
#include "Class_Client.h"

class Room {
public:
	Room() = default;
	Room(std::string name) : name(name), clients_in_the_room({}) {}

	std::string get_room_name();
	std::string get_users();
	std::vector<Clients*> retern_list_clients();
	void Add_new_client(Clients* cl);
	void Remove_from_Room(Clients* cl);
private:
	std::string name;
	std::vector<Clients*> clients_in_the_room;
	//история комнаты

};
#endif