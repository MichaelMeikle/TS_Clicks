//Common Header File
#include "common.h"
//Local Headers
#include "Log.h"

#define MAX_LOG_SIZE 30

std::list<Command> commandLog;
Log::Log() {
	//Default to true, set to false when the main doj_plugin file reaches its shutdown method
	programCrash = true;
	logLevel = LOG_ERROR;
}
Log::~Log() {
	 if (logFile.is_open() && programCrash) {
		logFile.flush();
		logFile.close();
	}
	else if (logFile.is_open()) {
		logFile.close();
		return;
	}
}
void Log::openFile(std::string filePath) {
	std::string temp = (filePath + "ts_clicks/logfile.txt");
	info(temp);
	logFile.open(temp);
	if (logFile.is_open()) {
		info("Log file opened");
	}
	else {
		error("Log file error, file not opened!");
	}
}
void Log::setLevel(int level) {
	logLevel = level;
}

void Log::info(std::string msg) {
	if (!(logLevel <= LOG_INFO))
		return;
	msg.insert(0, "[DOJRADIO] Info: ");
	printf(msg.c_str());
	printf("\n");
	msg.append("\n");
	logFile << msg.c_str();
}
void Log::warning(std::string msg) {
	if (!(logLevel <= LOG_WARNING))
		return;
	msg.insert(0, "[DOJRADIO] Warning: ");
	printf(msg.c_str());
	printf("\n");
	msg.append("\n");
	logFile << msg.c_str();
}
void Log::error(std::string msg) {
	if (!(logLevel <= LOG_ERROR))
		return;
	msg.insert(0, "[DOJRADIO] ERROR: ");
	printf(msg.c_str());
	printf("\n");
	msg.append("\n");
	logFile << msg.c_str();
}
//DisplayName-Command-UniqueID
void Log::command(std::string name, anyID senderID, std::string command) {
	//Ignore logging of uneccesary data commands which are already visible to the user via the client info data display
	if (command.find("data") != std::string::npos)
		return;

	Command logCom(name, senderID, command);
	commandLog.push_back(logCom);
	if (commandLog.size() > MAX_LOG_SIZE)
		commandLog.pop_front();
	info("Command Logged :: Command from ID: " + std::to_string(senderID) + " // Command: " + command);
}
void Log::printCommandLog() {
	printf("\n***DOJ Command Log***\n");
	printf("------------------\n");
	std::string boxOutput = "";
	int i = 0;
	for (Command command : commandLog) {
		i++;
		std::string temp = "";
		if (!(i == 1))
			temp.append("------------------------------------\n");
		//Format into readable time value
		long milli = command.timeCommand;
		//3600000 milliseconds in an hour
		long hr = milli / 3600000;
		milli = milli - 3600000 * hr;
		//60000 milliseconds in a minute
		long min = milli / 60000;
		milli = milli - 60000 * min;
		//1000 milliseconds in a second
		long sec = milli / 1000;
		temp.append("Time: " + std::to_string(hr) + ":" + std::to_string(min) + ":" + std::to_string(sec) + " // Command: " + command.command + " // Sender ID: " + std::to_string(command.senderID) + "// Display Name: " + command.displayName + "\n");
		boxOutput.append(temp);
		printf(temp.c_str());
	}
	printf("------------------\n");
	MessageBoxA(0, ("%s", boxOutput.c_str()), ("Command Log"), MB_ICONASTERISK | MB_OK);
}
void Log::safeClose() {
	programCrash = false;
}