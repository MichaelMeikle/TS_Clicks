#pragma once
//#include "radio.h"

class DOJPlugin {
public:
	static const char* pluginName;
	static const char* pluginAuthor;
	static const char* pluginDescription;
	static const char* pluginVersion;
	static const int apiVersion = 22;
	//TS3 Functions
	int init();
	void initLog();
	void shutdown();
	std::string infoData(uint64 serverConnectionHandlerID, uint64 id, enum PluginItemType type, char** data);
	void proccessCommand(uint64 serverConnectionHandlerID, std::string);
	//TS3 Events
	void onConnectStatusChangeEvent(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber);
	void onClientSelfVariableUpdate(uint64 serverConnectionHandlerID, int flag, std::string oldValue, std::string newValue);
	void onMenuItemEvent(uint64 serverConnectionHandlerID, enum PluginMenuType type, int menuItemID, uint64 selectedItemID);
	void onClientMoveEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID);
	void onHotkeyEvent(std::string keyword);
	void onJoinServerEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID);
	void onLeaveServerEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID);
	//Sound Handling
	void onEditPlaybackVoiceDataEvent(uint64 serverConnectionHandlerID, anyID clientID, short* samples, int sampleCount, int channels, const unsigned int* channelSpeakerArray, unsigned int* channelFillMask);
	void onCaputredVoiceDataEvent(uint64 serverConnectionHandlerID, short* samples, int sampleCount, int channels, int* edited);
	void onTalkStatusChangeEvent(uint64 serverConnectionHandlerID, int status, int isReceivedWhisper, anyID clientID);
	//Plugin Functions
	static void globalCommand(std::string command);
	static void channelCommand(std::string command);
	void commandTarget(std::string command, anyID clientID);
	void enablePlugin(uint64 serverConnectionHandlerID);
	//void disablePlugin(uint64 serverConnectionHandlerID);
	int channelManager(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID);
	void tenOneCleanup(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID);
};

void talkPermit(uint64 serverConnectionHandlerID);
void talkDeny(uint64 serverConnectionHandlerID);
void playWaveFile(std::string fileNameWithoutExtension);
void pluginDisable(uint64 serverConenctionHandlerID);
void showMenu(int menuID);
void hideMenu(int menuID);
std::string getClientChannelName(uint64 schId, anyID clientID);


//Used for new name state check method
enum specialNameStates {
	ACTIVE_DISPATCH,
	WEAZEL_NEWS,
	GROUP_SIX_SECURITY
};

//Special strings to look for in someone's name, used in checkNameForState() method
static const std::string specialCaseList[26] = {
	"[P]",
	"[S]",
	"[T]",
	"[FIRE]",
	"[EMS]",
	"[FIRE/EMS]",
	"[FE]",
	"[TAC]",
	"[911]",
	"[QA]",
	"[SA]",
	"[EVAL]",
	"(P)",
	"(S)",
	"(T)",
	"(FIRE)",
	"(EMS)",
	"(FIRE/EMS)",
	"(FE)",
	"(TAC)",
	"(911)",
	"(QA)",
	"(SA)",
	"(EVAL)",
	"WZL",
	"DL"
};
//Defines the last position of dispatch tags in the specialCaseList
#define END_DISPATCH_CHECK 24

//Lists for the channel manager
//Channels where the plugin is active by default
static const char *autoManageChannelList[5] =
{
	"Patrol Server 1*",
	"Patrol Server 2*",
	"Patrol Server 3*",
	"Patrol Server 4*",
	"Patrol Server 5*"
};
//Channels where your settings are preserved
static const char *talkOverrideStayList[6] =
{
	"10-1 Channel",
	"On Scene 1",
	"On Scene 2",
	"Civ Group*",
	"SAFA*",
	"Criminal*"
};
static std::string channelManagerList[5]{
	"Patrol Server 1*",
	"Patrol Server 2*",
	"Patrol Server 3*",
	"Patrol Server 4*",
	"Patrol Server 5*"
};
static std::string tenOneList[6]{
	"10-1 Channel",
	"On Scene 1",
	"On Scene 2",
	"Civ Group*",
	"SAFA*",
	"Criminal*"
};