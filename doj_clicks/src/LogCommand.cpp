#include "LogCommand.h"

Command::Command(std::string name,  anyID yourID, std::string commandStr) {
	senderID = yourID;
	targetID = NULL;
	displayName = name;
	command = commandStr;
	timeCommand = clock();

}
//TBD creating a more complex command logging/type logging system
Command::Command(anyID yourID, anyID theirID, std::string commandStr) {
	senderID = yourID; 
	targetID = theirID;
	command = commandStr;
	timeCommand = clock();
}