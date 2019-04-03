#include "common.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <list>
#include <time.h>
#include <stdint.h>
#include "doj_plugin.h"
#include "Log.h"

#define LOG_LEVEL LOG_INFO

static struct TS3Functions *ts3Functions;

Log console;

bool statusConnected = false;

const char* DOJPlugin::pluginName = "TS Clicks";
const char* DOJPlugin::pluginAuthor = "Michael M.";
const char* DOJPlugin::pluginVersion = "1.0";
const char* DOJPlugin::pluginDescription = "Automatic realistic mic clicks";
char pluginPath[512];

std::string pluginID;

bool enabled = false;
bool canEnable = false;

int DOJPlugin::init() {
	console.info("Initialization");
	pluginID = getPluginID();
	ts3Functions = getFunctionHandler();
	console.setLevel(LOG_LEVEL);
	ts3Functions->getPluginPath(pluginPath, 512, pluginID.c_str());
	std::string temp(pluginPath);
	console.info("FilePath Check: " + temp);
	return 0;
}

void DOJPlugin::initLog() {
	//Opens logfile
	char pluginPath[512];
	ts3Functions->getPluginPath(pluginPath, 512, pluginID.c_str());
	std::string tempPath = pluginPath;
	console.openFile(tempPath);
}

void DOJPlugin::shutdown() {
	console.warning("Shutdown");
}

void DOJPlugin::onConnectStatusChangeEvent(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber) {
	if (newStatus == STATUS_CONNECTED || newStatus == STATUS_CONNECTION_ESTABLISHED) {
		showMenu(GLOBAL_TOGGLE_PATROL);
	}
	else if (newStatus == STATUS_DISCONNECTED) {
		//hideMenu(GLOBAL_TOGGLE_PATROL);
	}
}

//Used to grab items to recall the infoData display after commands are received
static PluginItemType typeMenu;
static char** data2; //TBD removal of both grabbers as uneccesary

std::string DOJPlugin::infoData(uint64 serverConnectionHandlerID, uint64 id, enum PluginItemType type, char** data) {
	return "";
}

void DOJPlugin::proccessCommand(uint64 serverConnectionHandlerID, std::string data) {
}

/************************************SOUND HANDLERS*************************************************/
void DOJPlugin::onTalkStatusChangeEvent(uint64 serverConnectionHandlerID, int status, int isReceivedWhisper, anyID clientID) {
	if (!enabled)
		return;
	console.info("Talk status change from ID: " + std::to_string(clientID));

	anyID myID;
	ts3Functions->getClientID(serverConnectionHandlerID, &myID);
	if(clientID == myID) {
		if (status == STATUS_TALKING) {
			if (clock() % 5 == 0 || clock() % 3 == 0 || clock() % 7 == 0) {
				playWaveFile("MotorolaTalkQuery2");
				clock_t waitTime = clock();
				while (waitTime + 400 > clock());
				playWaveFile("mic_click_on");

			}
			else if (clock() % 2 == 0) {
				playWaveFile("mic_click_on");
	
			}
			else {
				playWaveFile("MotorolaTalkQuery");
				clock_t waitTime = clock();
				while (waitTime + 200 > clock());
				playWaveFile("mic_click_on");
			}
				

		}
		else if (status == STATUS_NOT_TALKING)
			playWaveFile("mic_click_off");
	}
	else {
		if (status == STATUS_NOT_TALKING)
			playWaveFile("mic_click_off");
	}

}

void DOJPlugin::onEditPlaybackVoiceDataEvent(uint64 serverConnectionHandlerID, anyID clientID, short* samples, int sampleCount, int channels, const unsigned int* channelSpeakerArray, unsigned int* channelFillMask) {
}

void DOJPlugin::onCaputredVoiceDataEvent(uint64 serverConnectionHandlerID, short* samples, int sampleCount, int channels, int* edited) {
}

/*************************************************************************************/

void DOJPlugin::onClientSelfVariableUpdate(uint64 serverConnectionHandlerID, int flag, std::string oldValue, std::string newValue) {
}

void DOJPlugin::onClientMoveEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID) {
	//Will return if call is from non-doj server
	anyID myID;
	ts3Functions->getClientID(serverConnectionHandlerID, &myID);
	uint64 myChannel;
	ts3Functions->getChannelOfClient(serverConnectionHandlerID, myID, &myChannel);

	if ((clientID == myID)) {
		if (channelManager(serverConnectionHandlerID, myID, oldChannelID, newChannelID) == 1) {
			//return;
		}
		else {
			//Auto Tac Controller
		}
	}
}
void DOJPlugin::onMenuItemEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID) {
	/* Global menu item was triggered. selectedItemID is unused and set to zero. */
	console.info("Plugin menu event");
	switch (menuItemID) {
			//Plugin Toggle
		case GLOBAL_TOGGLE_PATROL:
			if (!enabled) {
				enablePlugin(serverConnectionHandlerID);
			}
			else if (enabled) {
				pluginDisable(serverConnectionHandlerID);
			}
			break;
	}

}
void DOJPlugin::onHotkeyEvent(std::string keyword) {

}
void DOJPlugin::onJoinServerEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID) {
	

}
void DOJPlugin::onLeaveServerEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID) {
	console.info("Server leave event detected");
	anyID myID;
	ts3Functions->getClientID(serverConnectionHandlerID, &myID);
	if (clientID == myID) {
		//hideMenu(GLOBAL_TOGGLE_PATROL);
	}

}
void DOJPlugin::globalCommand(std::string command) {

}
void DOJPlugin::channelCommand(std::string command) {

}
void DOJPlugin::commandTarget(std::string command, anyID clientID) {
	
}

// Auto Mananger 
bool tenOne = false;
int DOJPlugin::channelManager(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID) {
	tenOne = false;

	console.info("Channel manager checking channel states");
	int transferCheck = 0;
	char *channelName;
	ts3Functions->getChannelVariableAsString(serverConnectionHandlerID, newChannelID, CHANNEL_NAME, &channelName);
	std::string chName(channelName);
	for (int i = 0; i < 5; i++) {
		if (chName.find(channelManagerList[i]) != std::string::npos) {
			enablePlugin(serverConnectionHandlerID);
			transferCheck++;
		}
	}
	for (int i = 0; i < 6; i++) {
		if (tenOneList[i].find(chName) != std::string::npos)
			tenOne = true;
	}
	//If you are not joining a patrol channel, it then checks to see if you are leaving a patrol channel
	ts3Functions->getChannelVariableAsString(serverConnectionHandlerID, oldChannelID, CHANNEL_NAME, &channelName);
	chName = channelName;
	for (int i = 0; i < 5; i++) {
		if (chName.find(channelManagerList[i]) != std::string::npos) {
			if (transferCheck != 1)
				pluginDisable(serverConnectionHandlerID);
			else
				transferCheck++;
			break;
		}
	}
	return (transferCheck >= 1 ? 1 : 0);
}
void DOJPlugin::tenOneCleanup(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID) {
	char *channelName;
	bool authorizedChannel = false;
	ts3Functions->getChannelVariableAsString(serverConnectionHandlerID, newChannelID, CHANNEL_NAME, &channelName);
	for (int i = 0; i < 5; i++) {
		if (strcmp(autoManageChannelList[i], channelName) == 0) {
			authorizedChannel = true;
		}
	}
	for (int i = 0; i < 6; i++) {
		if (strcmp(talkOverrideStayList[i], channelName) == 0) {
			authorizedChannel = true;
		}
	}
	if (!authorizedChannel) {
		console.info("10-1 cleanup complete");
	}
}

void DOJPlugin::enablePlugin(uint64 serverConnectionHandlerID) {
	console.warning("Plugin enabled");
	enabled = true;
	playWaveFile("MotorolaOn");
}

void talkPermit(uint64 serverConnectionHandlerID) {
	
}
void talkDeny(uint64 serverConnectionHandlerID) {
	
}

static std::string lastSound = "";
static clock_t timeLastSound = clock();
void playWaveFile(std::string fileNameWithoutExtension) {
	if (lastSound.compare(fileNameWithoutExtension) == 0 && clock() < timeLastSound + 1500) {
		return;
	}
	else {
		timeLastSound = clock();
		lastSound = fileNameWithoutExtension;
	}
	console.info("Playing wave file: " + fileNameWithoutExtension);
	
	std::string path = pluginPath;
	path.append("ts_clicks/sounds/");
	path.append(fileNameWithoutExtension);
	path.append(".wav");
	//console.info("Path Output: " + path);
	int errorOut = ts3Functions->playWaveFile(ts3Functions->getCurrentServerConnectionHandlerID(), path.c_str());
	if (errorOut != ERROR_ok) {
		console.error("Sound file failure: " + std::to_string(errorOut));
	}
}


void pluginDisable(uint64 serverConnectionHandlerID) {
	console.warning("Plugin disabled");
	enabled = false;
	playWaveFile("MotorolaOff");
}
void showMenu(int menuID) {
	ts3Functions->setPluginMenuEnabled(pluginID.c_str(), menuID, 1);
}
void hideMenu(int menuID) {
	ts3Functions->setPluginMenuEnabled(pluginID.c_str(), menuID, 0);
}


std::string getClientChannelName(uint64 schId,  anyID clientID) {
	anyID myID;
	ts3Functions->getClientID(schId, &myID);
	uint64 chID;
	if (ts3Functions->getChannelOfClient(schId, myID, &chID) != ERROR_ok)
		return "List Error";

	char *channelName;
	if (ts3Functions->getChannelVariableAsString(schId, chID, CHANNEL_NAME, &channelName) != ERROR_ok)
		return "Channel Name Error";

	return std::string (channelName);
}
