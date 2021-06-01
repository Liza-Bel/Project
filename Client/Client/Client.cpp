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

int main(int argc, char* argv[]) {

	Client client;
	client.Strart_Client();

	return 0;
}

