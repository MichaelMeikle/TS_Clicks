#pragma once
#include <string>
#include <time.h>
#include "common.h"
class Command {
public:
	anyID senderID;
	anyID targetID; //Not yet used
	std::string displayName;
	std::string command;
	std::string targetType; //Not yet used
	clock_t timeCommand;
	Command(std::string name, anyID yourID, std::string commandStr);
	Command(anyID yourID, anyID theirID, std::string commandStr); //Not yet used
};