#include "Class_Client.h"
#include <map>

struct tm newtime;
__time32_t aclock;

std::string Clients::get_room() {
	return room_name;
}
std::string Clients::get_user_name() {
	return user_name;
}
SOCKET Clients::get_connection()
{
	return connection;
}
void Clients::change_room(std::string names) {
	room_name = names;
}
void Clients::connectCallback(CallbackInterface * cb)
{
	m_cb = cb;
}

void Clients::Parse()
{
	std::map <std::string, int> commands;

	commands["@delete_room"] = Delete;
	commands["@add"] = Add;
	commands["@exit"] = Exit;
	commands["@create"] = Create;
	commands["@history"] = History;
	commands["@list_rooms"] = Listrooms;
	commands["@list_clients"] = Listclients;
	commands["@where"] = Where;

	int client_command_size;
	while (true)
	{
		
		if (recv(connection, (char*)&client_command_size, sizeof(int), NULL) == SOCKET_ERROR) {
			std::cout << "recv function failed with error " << WSAGetLastError() << std::endl;
			return;
		}
		char *client_command = new char[client_command_size + 1];
		client_command[client_command_size] = '\0';
		recv(connection, client_command, client_command_size, NULL);
		if (strcmp(client_command, "exit\n") == 0) {
			std::cout << "Client Disconnected." << std::endl;
			break;
		}
		if (client_command[0] == '@') {
			switch (commands[client_command]) {
			case Delete: {
				m_cb->Delete_Room(this);
			}
			case Where: {
				int msg_size = room_name.size();
				send(connection, (char*)&msg_size, sizeof(int), NULL);
				send(connection, room_name.c_str(), msg_size, NULL);
			}
			break;

			case Add: {
				std::string msg = "Select a room\n";
				int msg_size = msg.size();
				send(connection, (char*)&msg_size, sizeof(int), NULL);
				send(connection, msg.c_str(), msg_size, NULL);
				std::string list_room = m_cb->list_of_room();
				std::string s = "Rooms:" + list_room + " ";
				int s_size = s.size();
				send(connection, (char*)&s_size, sizeof(int), NULL);
				send(connection, s.c_str(), s_size, NULL);
				m_cb->Add_Client_in_the_Room(this);
			}
			break;

			case Exit: {
				m_cb->Leave_the_Room(this);
			}
			break;

			case Create: {
				std::string msg = "Enter a name for the new room:\n";
				int msg_size = msg.size();
				send(connection, (char*)&msg_size, sizeof(int), NULL);
				send(connection, msg.c_str(), msg_size, NULL);

				int room_name_size;
				recv(connection, (char*)&room_name_size, sizeof(int), NULL);
				char *room_name = new char[room_name_size + 1];
				room_name[room_name_size] = '\0';
				recv(connection, room_name, room_name_size, NULL);
				m_cb->Create_New_Room((std::string)room_name);
				delete[] room_name;
			}
			break;
			
			case History:
				//
				break;

			case Listrooms: {
				m_cb->list_of_room();
			}
			break;

			case Listclients: {
				m_cb->list_of_participants_in_the_room(this);
			}
			break;

			default: {
				std::string msg = "Command not found.\n";
				int msg_size = msg.size();
				send(connection, (char*)&msg_size, sizeof(int), NULL);
				send(connection, msg.c_str(), msg_size, NULL);
			}
			break;
			}
		}
		else {
			char buffer[32];
			errno_t errNum;
			_time32(&aclock);
			_localtime32_s(&newtime, &aclock);
			errNum = asctime_s(buffer, 32, &newtime);
			std::string msg = (std::string)buffer + this->user_name + ":" + (std::string)client_command;
			m_cb->Send_message(this, msg);
		}

	}
}
