#include "ClientData.h"

#include "plugin.h"
static struct TS3Functions *ts3Functions;

ClientData::ClientData() {
	ts3Functions = getFunctionHandler();
	primaryChID = 0;
	reset();
}
ClientData::ClientData(const ClientData &clone) {
	ts3Functions = getFunctionHandler();
	primaryChID = 0;
	clientID = clone.clientID;
	pluginVersion = clone.pluginVersion;
	for (int i = 0; i < DATA_ARRAY_SIZE; i++)
		dataArray[i] = clone.dataArray[i];
}
ClientData::ClientData(anyID id) {
	ts3Functions = getFunctionHandler();
	primaryChID = 0;
	clientID = id;
	reset();
}
ClientData::ClientData(anyID id, std::string metadata) {
	ts3Functions = getFunctionHandler();
	primaryChID = 0;
	clientID = id;
	buildData(metadata);
}
ClientData::~ClientData() {
	console("Destroying ClientData obj " + std::to_string(clientID) + "\n");
}
void ClientData::updateClientID(anyID id) {
	clientID = id;
}
//Resets data array (with exception of pluginInstall state) 
void ClientData::reset() {
	for (int i = 1; i < DATA_ARRAY_SIZE; i++) {
		dataArray[i] = 0;
	}
}

void ClientData::cloneObj(const ClientData &clone) {
	clientID = clone.clientID;
	pluginVersion = clone.pluginVersion;
	for (int i = 0; i < DATA_ARRAY_SIZE; i++)
		dataArray[i] = clone.dataArray[i];
}
//Data Commands
/***************************************/

void ClientData::buildData(std::string metadata) {
	std::stringstream ss(metadata);
	std::string arrayPacket = "";
	ss >> pluginVersion >> arrayPacket >> primaryChID >> uniqueID;
	for (int i = 0; i < DATA_ARRAY_SIZE; i++) {
		if (arrayPacket[i] == 'T')
			dataArray[i] = 1;
		else
			dataArray[i] = 0;
	}
}

std::string ClientData::getDataPacket() {
	std::stringstream ss;
	std::string packet("");
	for (int i = 0; i < DATA_ARRAY_SIZE; i++) {
		if (dataArray[i] == 1)
			packet.append("T");
		else
			packet.append("F");
	}
	ss << "data:" << pluginVersion << " " << packet << " " << primaryChID << " " << uniqueID << " ";
	return ss.str();
}

/***************************************/
std::string ClientData::getDataArray() {
	std::string packet = "";
	for (int i = 0; i < DATA_ARRAY_SIZE; i++) {
		if (dataArray[i] == 1)
			packet.append("T");
		else
			packet.append("F");
	}
	//console("Data Packet View Request: ");
	return packet;
}
std::string ClientData::versionNotification() {
	std::string returnVal("verNot:" + pluginVersion);
	return returnVal;
}
std::string ClientData::infoDataDisplay() {
	std::string data("[B]Version:[/B] " + pluginVersion + "\n"
		+ "[B]Plugin Status:[/B] " + (dataArray[pluginEnabled] ? "[I]Enabled[/I]\n" : "[I]Disabled[/I]\n")
		+ "[B]Talk Override:[/B] " + (dataArray[talkOverrideStatus] ? "[I]Enabled[/I]\n" : "[I]Disabled[/I]\n"));
	return data;
}
std::string ClientData::extractID(std::string str) {
	std::size_t const n = str.find_first_of("0123456789");
	if (n != std::string::npos)
	{
		std::size_t const m = str.find_first_not_of("0123456789", n);
		return str.substr(n, m != std::string::npos ? m - n : m);
	}
	return std::string();
}
//Not currently used, destruction currently triggered by a client exiting the server
std::string ClientData::destructCommand() {
	std::string command = "";
	command.append("dest:");
	command.append(std::to_string(clientID));
	return command;
}
//Checks for major differences between objects. Does not check plugin version difference
bool ClientData::equals(const ClientData &otherClient) {
	bool different = false;
	if (!(clientID == otherClient.clientID))
		different = true;
	else
		for (int i = 0; i < DATA_ARRAY_SIZE; i++) {
			if (dataArray[i] != otherClient.dataArray[i]) {
				different = true;
				break;
			}
		}
	return !different;
}
void ClientData::setVersion(std::string ver) {
	pluginVersion = ver;
}
void ClientData::console(std::string output) {
	std::string con("[DOJRADIO] ");
	con.append(output);
	printf(con.c_str());
}
void ClientData::setUniqueID(std::string UID) {
	uniqueID = UID;
}

/* LEGACY
	std::string packet = "data:";
	std::string version(pluginVersion);
	for (int i = (int) version.length(); i < VER_BUFF - 1; i++)
		version.append("x");
	version.append(":");
	packet.append(version);
	for (int i = 0; i < DATA_ARRAY_SIZE; i++) {
		if (dataArray[i] == 1)
			packet.append("T");
		else
			packet.append("F");
	}
	//packet.append(std::to_string(dataArray[transmitChannel]));
	*/

	/*
		if (metadata.length() > DATA_ARRAY_SIZE + 1) {
			console("Building with new command structure for TSID #" + std::to_string(clientID) + "\n");
			std::string versionInfo = metadata.substr(0, VER_BUFF);
			versionInfo.erase(std::remove(versionInfo.begin(), versionInfo.end(), 'x'), versionInfo.end());
			versionInfo.erase(std::remove(versionInfo.begin(), versionInfo.end(), ':'), versionInfo.end());
			pluginVersion = versionInfo;
			const char* meta = metadata.substr(VER_BUFF, DATA_ARRAY_SIZE).c_str();
			//console("DEBUG OUT:" + metadata.substr(VER_BUFF, DATA_ARRAY_SIZE) + "\n\n");
			for (int i = 0; i < DATA_ARRAY_SIZE; i++) {
				if (meta[i] == 'T')
					dataArray[i] = 1;
				else
					dataArray[i] = 0;
			}
			//Temp fix
			dataArray[pluginInstall] = 1;
		} //Old data packet handling
		else {
			console("Building with old command structure for TSID #" + std::to_string(clientID) + "\n");
			const char* meta = metadata.c_str();
			for (int i = 0; i < DATA_ARRAY_SIZE; i++) {
				if (meta[i] == 'T')
					dataArray[i] = 1;
				else
					dataArray[i] = 0;
			}
		}
		*/
		//dataArray[transmitChannel] = std::stoi(metadata.substr(8, metadata.length()));