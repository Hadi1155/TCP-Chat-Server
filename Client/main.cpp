#include <iostream>
#include <string.h>
#include<string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
	string ipAddress = "127.0.0.1"; //ip address server
	int port = 5400; //listening port

	//initialize winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "winsock can't start ! " << wsResult << endl;
		return;
	}

	//create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "can't create socket" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	//fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server! " << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}
	//do-while loop to send and receive data
	char buf[4096];
	string userInput;
	do
	{
		//input chat
		cout << "- ";
		getline(cin, userInput);

		if (userInput.size() > 0) //testing the user is typing
		{
			//send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				//wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					//echo response to console
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
				}
				
			}
			
		}
	} 
	while (userInput.size() > 0);

	//shutdown
	closesocket(sock);
	WSACleanup();
}