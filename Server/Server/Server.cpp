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
	std::vector <std::thread> th_vec;
	int count = 0;
	int name_client_size;
	std::string msg;
	int msg_size;
	char *name_client = nullptr;
	clients_connected.reserve(50);
	for (int i = 0; i < 50; i++) {
		new_Connection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
		count++;
		if (new_Connection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			std::cout << "Client Connected!\n";
			while (unique_name.size() != count) {
				msg = "Enter your name please:";
				msg_size = msg.size();
				send(new_Connection, (char*)&msg_size, sizeof(int), NULL);
				send(new_Connection, msg.c_str(), msg_size, NULL);

				int name_client_size;
				recv(new_Connection, (char*)&name_client_size, sizeof(int), NULL);
				name_client = new char[name_client_size + 1];
				name_client[name_client_size] = '\0';
				recv(new_Connection, name_client, name_client_size, NULL);
				if (Check_name((std::string)name_client)) {
					unique_name.push_back((std::string)name_client);
				}
			}

			int room_name_size;
			char *room_name = nullptr;
			if (rooms_server.size() == 0) {
				msg = "There are no rooms. Enter a name for the new room:";
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
				msg = "Select a room or create a new one:";
				msg_size = msg.size();
				send(new_Connection, (char*)&msg_size, sizeof(int), NULL);
				send(new_Connection, msg.c_str(), msg_size, NULL);
				std::string list_room = list_of_room();
				std::string s = "Rooms:" + list_room + " ";
				int s_size = s.size();
				send(new_Connection, (char*)&s_size, sizeof(int), NULL);
				send(new_Connection, s.c_str(), s_size, NULL);

				recv(new_Connection, (char*)&room_name_size, sizeof(int), NULL);
				room_name = new char[room_name_size + 1];
				room_name[room_name_size] = '\0';
				recv(new_Connection, room_name, room_name_size, NULL);

				if (list_room.find((std::string)room_name) == std::string::npos) {
					Create_New_Room((std::string) room_name);
				}

			}
			clients_connected.emplace_back((std::string)name_client, (std::string)room_name, new_Connection);
			for (int i = 0; i < rooms_server.size(); ++i) {
				if (rooms_server[i].get_room_name() == room_name) {
					rooms_server[i].Add_new_client(&clients_connected[clients_connected.size()-1]);
					break;
				}
			}
			
			clients_connected[clients_connected.size() - 1].connectCallback(this);
			th_vec.push_back(std::thread(&Clients::Parse, clients_connected[clients_connected.size() - 1]));
			delete[] name_client;
			delete[] room_name;
		}
	}
	for (int i = 0; i < th_vec.size(); ++i) {
		th_vec[i].join();
	}

}

void Server::Close_Server() {
	closesocket(sListen);
	WSACleanup();
	std::cout << "Server was stoped. You can close app" << std::endl;
}

void Server::Add_Client_in_the_Room(Clients* cl)
{
	int room_name_size;
	recv(cl->get_connection(), (char*)&room_name_size, sizeof(int), NULL);
	char *room_name = new char[room_name_size + 1];
	room_name[room_name_size] = '\0';
	recv(cl->get_connection(), room_name, room_name_size, NULL);

	for (auto &i : rooms_server) {
		if (i.get_room_name() == room_name)
			i.Add_new_client(cl);
	}
	cl->change_room(room_name);
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

void Server::Send_message(Clients * cl, std::string msg)
{
		std::vector<Clients*> tmp = {};
		for (auto &i : rooms_server) {
			if (i.get_room_name() == cl->get_room()) {
				tmp = i.retern_list_clients();
				break;
			}
		}

		int sms_size = msg.size();
		char *sms = new char[sms_size + 1];
		sms[sms_size] = '\0';
		for (int i = 0; i < msg.size(); i++)
			sms[i] = msg[i];

		for (size_t i = 0; i < tmp.size(); ++i) {
			if (cl->get_user_name() == tmp[i]->get_user_name())
				continue;
			int msg_size = msg.size();
			send(tmp[i]->get_connection(), (char*)&sms_size, sizeof(int), NULL);
			send(tmp[i]->get_connection(), sms, sms_size, NULL);
		}
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