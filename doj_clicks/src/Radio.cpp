#include "Radio.h"
Radio::Radio() {

}
Radio::Radio(int departmentID) {
	switch (departmentID) {
	case LSPD:
		primaryChannelID = San_Andreas_SW;
		secondaryChannelID = LS_Citywide;
		break;
	case BCSO:
		primaryChannelID = San_Andreas_SW;
		secondaryChannelID = County_Car2Car;
		break;
	case HWY:
		primaryChannelID = San_Andreas_SW;
		secondaryChannelID = HWY_Patrol;
		break;
	case LSFD:
		primaryChannelID = San_Andreas_SW;
		secondaryChannelID = LSFD_Fireground;
		break;
	case Communications:
		primaryChannelID = San_Andreas_SW;
		secondaryChannelID = DispatchChat;
		break;
	case Civ:
		primaryChannelID = Civilian_Ops;
		secondaryChannelID = NULL; //Civs have secondarys available to them but not neccesary by default
		break;
	}
	agency = departmentID;
	for (int i = 0; i < sizeof(monitorList); i++)
		monitorList[i] = NULL;
}
Radio::Radio(int primaryDefault, int secondaryDefault) {
	primaryChannelID = primaryDefault;
	secondaryChannelID = secondaryDefault;
}

void Radio::changePrimaryChannel(int channelID) {
	primaryChannelID = channelID;
}

void Radio::changeSecondaryChannel(int channelID) {
	secondaryChannelID = channelID;
}
//Returns true if added, returns false if no space
bool Radio::addToMonitorList(int channelID) {
	for (int i = 0; i < sizeof(monitorList); i++)
		if (monitorList[i] == NULL) {
			monitorList[i] = channelID;
			return true;
		}
	return false;
}
void Radio::removeFromMonitorList(int channelID) {
	for (int i = 0; i < sizeof(monitorList); i++)
		if (monitorList[i] == channelID)
			monitorList[i] = NULL;
}
/************************************/

int Radio::getPrimary() {
	return primaryChannelID;
}
int Radio::getSecondary() {
	return secondaryChannelID;
}
int* Radio::getMonitorList() {
	return monitorList;
}
bool Radio::onMonitorList(int channelID) {
	for (int i = 0; i < sizeof(monitorList); i++) {
		if (monitorList[i] == channelID)
			return true;
	}
	return false;
}