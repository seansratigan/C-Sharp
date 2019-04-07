#pragma once
#include <iostream>
#include <string>
const std::string ws = "\x20";
int nth(int n, std::string e, std::string s);
std::string getNick(std::string s);
std::string getHost(std::string s);
std::string getUser(std::string s);
std::string getMsgType(std::string s);
std::string getMessage(std::string s);
bool isChannel(std::string s);
char *stringToChar(std::string in);
std::string getParameters(std::string message);