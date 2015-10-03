#include "fieldproperties.h"

FieldProperties::FieldProperties() {
	name = "SPL2013"; // SPL2013 for default-instance
	borderStripWidth = 70;
	centerCircleDiameter = 150;
	fieldLength = 900;
	fieldWidth = 600;
	lineWidth = 5;

	penaltyAreaLength = 60;
	penaltyAreaWidth = 220;
	penaltyMarkDistance = 180;
	penaltyMarkSize = 10;

	goalPostSize = 10;
	goalRoom = 50;
	goalWidth = 150;

	ballSize = 15;
	halftimeDuration = 10;
	playerSize = 30;
	teamSize = 5;
}

FieldProperties::~FieldProperties() {}

/**
* @brief Saves the object as JSON.
*
*
*/
void FieldProperties::write(QJsonObject &json) const {
	json["name"] = name;
	json["borderStripWidth"] = borderStripWidth;
	json["centerCircleDiameter"] = centerCircleDiameter;
	json["length"] = fieldLength;
	json["lineWidth"] = lineWidth;
	json["width"] = fieldWidth;

	json["penaltMarkSize"] = penaltyMarkSize;
	json["penaltyAreaLength"] = penaltyAreaLength;
	json["penaltyAreaWidth"] = penaltyAreaWidth;
	json["penaltyMarkDistance"] = penaltyMarkDistance;

	json["goalPostSize"] = goalPostSize;
	json["goalRoom"] = goalRoom;
	json["goalWidth"] = goalWidth;

	json["ballSize"] = ballSize;
	json["halfDuration"] = halftimeDuration;
	json["playerSize"] = playerSize;
	json["teamSize"] = teamSize;
}

/**
* @brief Reads a object from JSON.
*
*/
void FieldProperties::read(const QJsonObject &json) {
	name = json["name"].toString();

	borderStripWidth = json["borderStripWidth"].toInt();
	centerCircleDiameter = json["centerCircleDiameter"].toInt();
	fieldLength = json["length"].toInt();
	fieldWidth = json["width"].toInt();
	lineWidth = json["lineWidth"].toInt();

	penaltyAreaLength = json["penaltyAreaLength"].toInt();
	penaltyAreaWidth = json["penaltyAreaWidth"].toInt();
	penaltyMarkDistance = json["penaltyMarkDistance"].toInt();
	penaltyMarkSize = json["penaltMarkSize"].toInt();

	goalPostSize = json["goalPostSize"].toInt();
	goalRoom = json["goalRoom"].toInt();
	goalWidth = json["goalWidth"].toInt();

	ballSize = json["ballSize"].toInt();
	halftimeDuration = json["halfDuration"].toInt();
	playerSize = json["playerSize"].toInt();
	teamSize = json["teamSize"].toInt();
}