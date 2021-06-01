#include "Class_Room.h"

std::string Room::get_room_name() {
	return name;
}
std::string Room::get_users() {
	std::string names = "";
	for (auto &i : clients_in_the_room) {
		names += i->get_user_name();
		names += " ";
	}
	return names;
}

void Room::Add_new_client(Clients* cl) {
	clients_in_the_room.push_back(cl);
}

void Room::Remove_from_Room(Clients* cl) {
	auto iter = std::find(clients_in_the_room.begin(), clients_in_the_room.end(), cl);
	if (iter != clients_in_the_room.end())
		clients_in_the_room.erase(iter);
}