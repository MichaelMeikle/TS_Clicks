#pragma once
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include "LogCommand.h"

enum LOG_LEVELS {
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};
class Log {
	int logLevel;
	std::ofstream logFile;
	bool programCrash;
public:
	Log();
	~Log();
	void openFile(std::string filePath);
	void setLevel(int level);
	void error(std::string msg);
	void warning(std::string msg);
	void info(std::string msg);
	void command(std::string name, anyID senderID, std::string commandStr);
	void printCommandLog();
	void safeClose();
};