#include "Class_Client.h"

std::string Clients::get_room() {
	return room_name;
}
std::string Clients::get_user_name() {
	return user_name;
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
	int msg_size;
	while (true)
	{
		recv(ñonnection, (char*)&msg_size, sizeof(int), NULL);
		char *msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(ñonnection, msg, msg_size, NULL); 
		if (msg[0] == '@') {
			
		}
		else {

		}

		delete[] msg;
	}
	
}
