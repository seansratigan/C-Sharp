#include "pch.h"
#include <iostream>
#include <string>

const std::string ws = "\x20"; // Whitespace (or space)

int nth(int n, std::string e, std::string s)
{
	if (s.find(e) != std::string::npos)
	{
		std::string::size_type i = s.find(e);     // Find the first occurrence

		int j;
		for (j = 1; j < n && i != std::string::npos; ++j)
			i = s.find(e, i + 1); // Find the next occurrence

		if (j == n)
			return(i);
		else
			return(-1);
	}
}
// Find a Nick from an incoming message
std::string getNick(std::string s)
{
	std::size_t pos = s.find(":") + 1;
	std::size_t eol = s.find("!");
	std::size_t len = eol - pos;
	std::string nick = s.substr(pos, len);
	return 	nick;
}

// Find a Hostname from an incoming message
std::string getHost(std::string s)
{
	std::size_t pos = s.find("@") + 1;
	std::size_t eol = nth(1, ws, s);
	std::size_t len = eol - pos;
	std::string host = s.substr(pos, len);
	return host;
}

// Find a User Name from an incoming message
std::string getUser(std::string s)
{
	std::size_t pos = s.find("!") + 1;
	std::size_t eol = s.find("@");
	std::size_t len = eol - pos;
	std::string user = s.substr(pos, len);
	return user;
}

// Find if the message is a Private Message, Channel Message, or Notice.
std::string getMsgType(std::string s)
{
	std::size_t pos = nth(1, ws, s) +1;
	std::size_t eol = nth(2, ws, s);
	std::size_t len = eol - pos;
	std::string type = s.substr(pos, len);
	return type;
}

std::string getMessage(std::string s)
{
	std::size_t pos = nth(3, ws, s) + 1;
	std::size_t eol = nth(1, ws, s);
	std::size_t len = eol - pos;
	std::string message = s.substr(pos, len);
	return message;
}

// Returns true if the message came from a Channel.
bool isChannel(std::string s)
{
	std::size_t pos = nth(2, ws, s);
	std::size_t eol = nth(2, ":", s);
	std::size_t len = eol - pos;
	std::string type = s.substr(pos, len);
	if (s.find("#", 0) != std::string::npos) { return true; }
	return false;
}

//Convert string argument to char
char *stringToChar(std::string in)
{
	char out[512]{};

	int i;
	for (i = 0; i < in.length(); i++) {
		out[i] = in[i];
	}

	return out;
}

// Find how many occurances of a substring in a string
std::string getParameters(std::string message)
{
	std::string substring{};
	std::string parameter[25];
	int i{};
	do {
		std::size_t pos = message.find("\x01");
		substring = message.substr(0, pos);
		parameter[i] = substring;
		message.erase(0, pos);
		i++;
	} while (message.find("\x01") != std::string::npos);

}