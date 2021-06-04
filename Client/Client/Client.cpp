#include "Client.h"

void Client::Strart_Client()
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(1112);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) { 
		std::cout << "Error: failed connect to server.\n";
		return;
	}
	std::cout << "Connected!\n";

}

void Client::Receiving_Messages()
{
	int msg_size;
	while (true)
	{
		if (recv(Connection, (char*)&msg_size, sizeof(int), NULL) == SOCKET_ERROR) {
			std::cout << "recv function failed with error: " << WSAGetLastError() << std::endl;
			return;
		}
		char *msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connection, msg, msg_size, NULL);
		if (strcmp(msg, "exit\n") == 0) {
			std::cout << "Server disconnected." << std::endl;
			return;
		}
		std::cout << msg << std::endl;
		delete[] msg;
	}
}

void Client::Send_Message()
{
	std::string msg1;
	while (true)
	{
		std::getline(std::cin, msg1);
		int msg_size = msg1.size();
		send(Connection, (char*)&msg_size, sizeof(int), NULL); 
		if (send(Connection, msg1.c_str(), msg_size, 0) == SOCKET_ERROR) {
			std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
			return;
		}
		if (strcmp(msg1.c_str(), "exit") == 0) {
			std::cout << "Thank you for using the application" << std::endl;
			break;
		}
		Sleep(10);
	}
}

int main(int argc, char* argv[]) {

	Client client;
	client.Strart_Client();
	std::thread t(&Client::Receiving_Messages, client);
	std::thread t1(&Client::Send_Message, client);
	t.join();
	t1.join();
	return 0;
}

