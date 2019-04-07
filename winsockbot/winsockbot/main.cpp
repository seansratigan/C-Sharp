#include "pch.h"
#include "WinSock2.h"
#include "ws2tcpip.h"
#include "Windows.h"
#include <fstream>
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include <string>
#include "time.h"
#include "message_handler.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6667"

// PROTOTYPES
void raw(std::string in);
void sendConnectionReg();
void commandParser(std::string nick, std::string user, std::string host, std::string messageType, std::string message, bool isChannel);
std::string convertToString(char in[]);
//char *stringToChar(std::string in);
//std::string getHost(std::string s);
//std::string getNick(std::string s);
//int nth(int n, std::string e, std::string s);
//std::string getUser(std::string s);
void sendError();
void shutdown();

// CONFIG

const std::string BOT_NICK = "Boopewt";
const std::string BOT_USER = "cppclient";
const std::string BOT_MODE = "8";
const std::string BOT_REAL_NAME = "Unfinished IRC Client";

WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo *result = NULL,
	*ptr = NULL,
	hints;
char *sendbuf{};

char recvbuf[DEFAULT_BUFLEN];

int err;
int recvbuflen = DEFAULT_BUFLEN;

int __cdecl main(int argc, char **argv)
{
	// Validate the parameters
	if (argc != 2) {
		std::cout << "usage: " << argv[0] << " server.address\n";
		return 1;
	}

	// Initialize Winsock
	system("TITLE Initializing Winsock");
	err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		std::cout << "Failed to Initialize Winsock. Error Code: " << err << "\n";
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	system("TITLE Resolving address");
	err = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (err != 0) {
		std::cout << "Failed to resolve server address and port. Error Code: " << err << "\n";
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	system("TITLE Trying hosts");
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			std::cout << "Failed to create socket. Error Code: " << WSAGetLastError() << "\n";
			shutdown();
			return 1;
		}
		// Attempt to connect to the server.
		err = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (err == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	// Do we have a connection??
	if (ConnectSocket == INVALID_SOCKET) {
		std::cout << "Unable to connect to server.\n";
		shutdown();
		return 1;
	}
	// If yes, lets send the Connection Registration.
	else
	{
		sendConnectionReg();
		system("TITLE CONNECTED: Waiting for /MOTD");
	}

	do {

		err = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (err > 0)
		{
			// Holds the position of the last element in recvLine[].
			int i{ 0 }; // We'll use this later to parse words. Resets after loop.
			// Convert recvbuf to string, because: Fuck char right in the pussy.
			std::string recvstr = convertToString(recvbuf);
			std::string recvLines[25];
			//std::string recvWords[150]; outdated
			do {// Looping until no more CR & LF is found in the string.
				// Find the end of the line in the buffer by locating the CR & LF (\r & \n)
				// This will return the position they begin. Add 2 to that position.
				std::size_t pos = recvstr.find("\r\n");
				// Creating substring with the CR & LF (+ 2)
				std::string substring{};
				substring = recvstr.substr(0, pos + 2);
				// Copy this substring to an Array at Nth position.
				recvLines[i] = substring;
				// Erasing from endOfLine + 2 positions to erase the CR & LF.
				recvstr.erase(0, pos + 2);
				i++;
			} while (recvstr.find("\r\n") != std::string::npos);

			// Clear the buffer
			memset(recvbuf, 0, sizeof(recvbuf));
			// Life gives you sentences, make words.
			std::string line;
			for (int iii = 0; iii < i; iii++)
			{
				// Copy the element to a string
				line = recvLines[iii];
				std::cout << recvLines[iii];
				std::size_t pos;
				std::size_t eol;
				std::size_t len;
				std::string nick{};
				std::string user{};
				std::string host{};
				std::string msgType{};
				std::string channel{};
				std::string ping{};
				std::string message{};
				std::string parameter[30];
				bool isChan;
				if (line.find("!") != std::string::npos && line.find("@") != std::string::npos)
				{ 
					if (line.find("!") < line.find("@") && line.find("@") < line.find(nth(1, ws, line)))
					{
						nick = getNick(line); user = getUser(line); host = getHost(line); msgType = getMsgType(line);
						isChan = isChannel(line); message = getMessage(line);

						std::string substrang{};
						//std::string parameter[25];
						// Its crashing when i do a CTCP Version. its this loop causing it..
						// I'm too fucking tired to figure it out right now..
						// Good night.
						int v{};
						std::string newmessage{message};
						do {
							int pussy = newmessage.find("\x01");
							substrang = newmessage.substr(0, pussy);
							parameter[v] = substrang;
							newmessage.erase(0, pussy);
							v++;
						} while (newmessage.find("\x01") != std::string::npos);


						//raw("PRIVMSG #Dev " + *parameter + "\r\n");
						commandParser(nick, user, host, msgType, message, isChan);

					}
				}
				if (line.find("PING") != std::string::npos && line.find("PRIVMSG") == std::string::npos)
				{
					pos = line.find("PING") + 5;
					eol = line.find("\r\n");
					ping = "PONG " + line.substr(pos, eol);
					raw(ping);
				}
			} // end iii for loop

		} // end if(err)


		else if (err == 0)
		{
			std::cout << "Connection closed.\n";
		}
		else
		{
			std::cout << "recv FAILED. Error code: " << WSAGetLastError() << "\n";
		}



	} while (err > 0); // END do while



	// cleanup
	shutdown();
	return 0;
}

void commandParser(std::string nick, std::string user, std::string host, std::string messageType, std::string message, bool isChannel)
{

	
	if (isChannel)
	{

	}
	if (messageType == "NOTICE")
	{

	}
	if (messageType == "PRIVMSG")
	{
		std::cout << nick << " " << user << " " << host << " " << messageType << " " << message << " " << isChannel;
		if (message.find("\x01PING") != std::string::npos)
		{
			raw("NOTICE " + nick + " " + message);
		}
		if (message.find("\x01VERSION") != std::string::npos)
		{
			raw("NOTICE " + nick + " " + "\x01VERSION\x01 Don't feed the liberals.");
		}
		if (message.find("!sop") != std::string::npos)
		{
			raw("oper cppork EEEppp111");
		}
		if (message.find("!join") != std::string::npos)
		{
			raw("JOIN #Dev");
		}
	}

}
// TODO: Build Enum class of error codes to cout
void sendError() 
{
	std::cout << "Send FAILED, Error Code: " << WSAGetLastError() << "\r\n";
}
void shutdown()
{
	closesocket(ConnectSocket);
	WSACleanup();
}

// Send to the socket
void raw(std::string in)
{
	char *bufchar = stringToChar(in + "\r\n");
	err = send(ConnectSocket, bufchar, (int)strlen(bufchar), 0);
	std::cout << "-> TO ->" << bufchar << "\n";
	if (err == SOCKET_ERROR) {
		std::cout << "Failed to send message to server. Error Code: " << WSAGetLastError() << "\n";
		shutdown();
	}
}

void sendConnectionReg()
{
	Sleep(650);
	raw("NICK " + BOT_NICK);
	Sleep(150);
	raw("USER " + BOT_USER + " " + BOT_MODE + " * " + ":" + BOT_REAL_NAME);
}

std::string convertToString(char in[])
{
	std::string out = in;
	return out;
}

