#include "Server.h"

void Server::Start_Server() {
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(1112);
	addr.sin_family = AF_INET;

	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);
}

void Server::Connect_Client()
{
	SOCKET new_Connection;
	for (int i = 0; i < 100; i++) {
		new_Connection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (new_Connection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			std::cout << "Client Connected!\n";
			std::string msg = "Enter your name please:\n";
			int msg_size = msg.size();
			send(new_Connection, (char*)&msg_size, sizeof(int), NULL);
			send(new_Connection, msg.c_str(), msg_size, NULL);

			int name_client_size;
			recv(new_Connection, (char*)&name_client_size, sizeof(int), NULL);
			char *name_client = new char[name_client_size + 1];
			msg[name_client_size] = '\0';
			recv(new_Connection, name_client, name_client_size, NULL);


			int room_name_size;
			char *room_name = nullptr;
			if (rooms_server.size() == 0) {
				msg = "There are no rooms. Enter a name for the new room:\n";
				msg_size = msg.size();
				send(new_Connection, (char*)&msg_size, sizeof(int), NULL);
				send(new_Connection, msg.c_str(), msg_size, NULL);

				recv(new_Connection, (char*)&room_name_size, sizeof(int), NULL);
				room_name = new char[room_name_size + 1];
				room_name[room_name_size] = '\0';
				recv(new_Connection, room_name, room_name_size, NULL);
				Create_New_Room((std::string) room_name);

			}
			else {
				msg = "Select a room\n";
				msg_size = msg.size();
				send(new_Connection, (char*)&msg_size, sizeof(int), NULL);
				send(new_Connection, msg.c_str(), msg_size, NULL);

				std::string s = list_of_room();
				int s_size = s.size();
				send(new_Connection, (char*)&s_size, sizeof(int), NULL);
				send(new_Connection, s.c_str(), s_size, NULL);

				recv(new_Connection, (char*)&room_name_size, sizeof(int), NULL);
				room_name = new char[room_name_size + 1];
				room_name[room_name_size] = '\0';
				recv(new_Connection, room_name, room_name_size, NULL);

			}

			Clients new_Client((std::string)name_client, (std::string)room_name, new_Connection);
			clients_connected.push_back(new_Client);
			for (int i = 0; i < rooms_server.size(); ++i) {
				if (rooms_server[i].get_room_name == room_name) {
					rooms_server[i].Add_new_client(&new_Client);
					break;
				}
			}
			new_Client.Parse();

			delete[] name_client;
			delete[] room_name;
		}
	}
}

void Server::Close_Server() {
	closesocket(sListen);
	WSACleanup();
	std::cout << "Server was stoped. You can close app" << std::endl;
}

void Server::Add_Client_in_the_Room(std::string index, Clients* cl)
{
	for (auto &i : rooms_server) {
		if (i.get_room_name() == index)
			i.Add_new_client(cl);
	}
	cl->change_room(index);
}

void Server::Create_New_Room(std::string name)
{
	Room new_room(name);
	rooms_server.push_back(new_room);
}

void Server::Delete_Room(Clients* cl)
{
	std::string name = cl->get_room();
	for (auto &i : clients_connected) {
		if (i.get_room() == name)
			i.change_room("lobby");
	}

	for (int i = 0; i < rooms_server.size(); ++i) {
		if (rooms_server[i].get_room_name() == name) {
			rooms_server.erase(rooms_server.begin() + i);
			break;
		}
	}
}

void Server::Leave_the_Room(Clients* cl)
{
	for (int i = 0; i < rooms_server.size(); ++i) {
		if (rooms_server[i].get_room_name() == cl->get_room()) {
			rooms_server[i].Remove_from_Room(cl);
			break;
		}
	}
	cl->change_room("lobby");

}

bool Server::Check_name(std::string user_name)
{
	return std::find(unique_name.begin(), unique_name.end(), user_name) == unique_name.end();
}

std::string Server::list_of_room()
{
	std::string list_names = "";
	for (auto &i : rooms_server) {
		list_names += i.get_room_name();
		list_names += " ";
	}
	return list_names;
}

std::string Server::list_of_participants_in_the_room(Clients* cl)
{
	for (auto &i : rooms_server) {
		if (i.get_room_name() == cl->get_room())
			return i.get_users();
	}
}


int main(int argc, char* argv[]) {
	Server test;
	test.Start_Server();
	test.Connect_Client();

	test.Close_Server();
	return 0;
}