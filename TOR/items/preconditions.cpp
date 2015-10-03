#include "preconditions.h"
#include <QJsonObject>
#include <QDebug>

/**
* @brief Class for saving the PreConditions.
*
*
*/

PreConditions::PreConditions(QObject *parent) {
	name = QString::number(rand());

	modeToggle = false;
	timeToggle = false;
	remainingTimeToggle = false;
	halftimeToggle = false;
	ownGoalsToggle = false;
	oppGoalsToggle = false;
	difGoalsToggle = false;
	activePlayerToogle = false;
	activeGoalieToggle = false;
	activeOpponentToogle = false;
	activeOppGoalieToggle = false;

	gamemode = STATE_PLAYING;

	gameTimeMin = 0;
	gameTimeMax = 0;
	gameTimeRemainingMin = 0;
	gameTimeRemainingMax = 0;

	halftime = ALL;

	ownGoalsMin = 0;
	ownGoalsMax = 0;
	oppGoalsMin = 0;
	oppGoalsMax = 0;
	difGoalsMin = 0;
	difGoalsMax = 0;
	activePlayerMin = 0;
	activePlayerMax = 0;
	activeGoalie = true;
	activeOpponentMin = 0;
	activeOpponentMax = 0;
	activeOpponentGoalie = true;
	note = "";
	fieldProperties = new FieldProperties;
	fieldtype = fieldProperties->getName();
}

/**
* @brief Writes the object to JSON.
*
*
*/
void PreConditions::write(QJsonObject &json) const {
	json["name"] = name;
	json["note"] = note;
	json["fieldtype"] = fieldtype;

	if (modeToggle) {
		QMetaObject MetaGamemode = this->staticMetaObject;
		QMetaEnum MetaEnumGamemode =
		MetaGamemode.enumerator(MetaGamemode.indexOfEnumerator("Gamemode"));
		json["gamemode"] = QString::fromUtf8(MetaEnumGamemode.valueToKey(gamemode));
	}

	if (timeToggle) {
		json["gameTimeMin"] = gameTimeMin;
		json["gameTimeMax"] = gameTimeMax;
	}
	if (remainingTimeToggle) {
		json["gameTimeRemainingMin"] = gameTimeRemainingMin;
		json["gameTimeRemainingMax"] = gameTimeRemainingMax;
	}

	if (halftimeToggle) {
		QMetaObject MetaHalftime = this->staticMetaObject;
		QMetaEnum MetaEnumHalftime =
		MetaHalftime.enumerator(MetaHalftime.indexOfEnumerator("Halftime"));
		json["halftime"] = QString::fromUtf8(MetaEnumHalftime.valueToKey(halftime));
	}

	if (ownGoalsToggle) {
		json["ownGoalsMin"] = ownGoalsMin;
		json["ownGoalsMax"] = ownGoalsMax;
	}

	if (oppGoalsToggle) {
		json["oppGoalsMin"] = oppGoalsMin;
		json["oppGoalsMax"] = oppGoalsMax;
	}

	if (difGoalsToggle) {
		json["difGoalsMin"] = difGoalsMin;
		json["difGoalsMax"] = difGoalsMax;
	}

	if (activePlayerToogle) {
		json["activePlayerMin"] = activePlayerMin;
		json["activePlayerMax"] = activePlayerMax;
	}

	if (activeGoalieToggle) {
		json["activeGoalie"] = activeGoalie;
	}

	if (activeOpponentToogle) {
		json["activeOpponentMin"] = activeOpponentMin;
		json["activeOpponentMax"] = activeOpponentMax;
	}
	if (activeOppGoalieToggle) {
		json["activeOpponentGoalie"] = activeOpponentGoalie;
	}
}

/**
* @brief Reads an object from JSON.
*
*
*/
void PreConditions::read(const QJsonObject &json) {
	name = json["name"].toString();
	note = json["note"].toString();
	fieldtype = json["fieldtype"].toString();

	if (json.contains("gamemode")) {
		modeToggle = true;
		QMetaObject MetaGamemode = this->staticMetaObject;
		QMetaEnum MetaEnumGamemode =
		MetaGamemode.enumerator(MetaGamemode.indexOfEnumerator("Gamemode"));
		gamemode =
		static_cast<PreConditions::Gamemode>(MetaEnumGamemode.keysToValue(
		json["gamemode"].toString().toUtf8().toUpper()));
	}
	if (json.contains("gameTimeMin")) {
		timeToggle = true;
		gameTimeMin = json["gameTimeMin"].toInt();
	}
	if (json.contains("gameTimeMax")) {
		timeToggle = true;
		gameTimeMax = json["gameTimeMax"].toInt();
	}

	if (json.contains("gameTimeRemainingMin")) {
		remainingTimeToggle = true;
		gameTimeRemainingMin = json["gameTimeRemainingMin"].toInt();
	}
	if (json.contains("gameTimeRemainingMax")) {
		remainingTimeToggle = true;
		gameTimeRemainingMax = json["gameTimeRemainingMax"].toInt();
	}
	if (json.contains("halftime")) {
		halftimeToggle = true;
		QMetaObject MetaHalftime = this->staticMetaObject;
		QMetaEnum MetaEnumHalftime =
		MetaHalftime.enumerator(MetaHalftime.indexOfEnumerator("Halftime"));

		halftime = static_cast<PreConditions::Halftime>(MetaEnumHalftime.keyToValue(
		json["halftime"].toString().toUtf8().toUpper()));
	}
	if (json.contains("ownGoalsMin")) {
		ownGoalsToggle = true;
		ownGoalsMin = json["ownGoalsMin"].toInt();
	}
	if (json.contains("ownGoalsMax")) {
		ownGoalsToggle = true;
		ownGoalsMax = json["ownGoalsMax"].toInt();
	}
	if (json.contains("oppGoalsMin")) {
		oppGoalsToggle = true;
		oppGoalsMin = json["oppGoalsMin"].toInt();
	}
	if (json.contains("oppGoalsMax")) {
		oppGoalsToggle = true;
		oppGoalsMax = json["oppGoalsMax"].toInt();
	}
	if (json.contains("difGoalsMin")) {
		difGoalsToggle = true;
		difGoalsMin = json["difGoalsMin"].toInt();
	}
	if (json.contains("difGoalsMax")) {
		difGoalsToggle = true;
		difGoalsMax = json["difGoalsMax"].toInt();
	}
	if (json.contains("activePlayerMin")) {
		activePlayerToogle = true;
		activePlayerMin = json["activePlayerMin"].toInt();
	}
	if (json.contains("activePlayerMax")) {
		activePlayerToogle = true;
		activePlayerMax = json["activePlayerMax"].toInt();
	}
	if (json.contains("activeGoalie")) {
		activeGoalieToggle = true;
		activeGoalie = json["activeGoalie"].toBool();
	}

	if (json.contains("activeOpponentMin")) {
		activeOpponentToogle = true;
		activeOpponentMin = json["activeOpponentMin"].toInt();
	}
	if (json.contains("activeOpponentMax")) {
		activeOpponentToogle = true;
		activeOpponentMax = json["activeOpponentMax"].toInt();
	}
	if (json.contains("activeOpponentGoalie")) {
		activeOppGoalieToggle = true;
		activeOpponentGoalie = json["activeOpponentGoalie"].toBool();
	}

	activeGoalieToggle = json.contains("activeGoalie");
	activeOpponentToogle = (json.contains("activeOpponentMin") ||
	json.contains("activeOpponentMax"));
	activeOppGoalieToggle = json.contains("activeOpponentGoalie");
}

/**
* @brief Remove some impossible values.
*
*
*/
void PreConditions::removeImpossibleFieldProperties(FieldProperties *value) {
	fieldProperties = value;

	if (gameTimeMax == 0) {
		gameTimeMax = fieldProperties->getHalftimeDuration();
	}
	if (gameTimeRemainingMax == 0) {
		gameTimeRemainingMax = fieldProperties->getHalftimeDuration();
	}
	if (activePlayerMax == 0) {
		activePlayerMax = fieldProperties->getTeamSize();
	}
	if (activeOpponentMax == 0) {
		activeOpponentMax = fieldProperties->getTeamSize();
	}
	if(fieldProperties->getName() != "SPL2013"){
		fieldtype = fieldProperties->getName();
	}
}
