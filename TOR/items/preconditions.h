#ifndef PRECONDITIONS_H
#define PRECONDITIONS_H

#include <QObject>
#include <QMetaEnum>
#include "fieldproperties.h"

class PreConditions : public QObject {
	Q_OBJECT
	Q_ENUMS(Gamemode)
	Q_ENUMS(Halftime)

public:
	explicit PreConditions(QObject *parent = 0);

	void write(QJsonObject &json) const;
	void read(const QJsonObject &json);

	enum Gamemode {STATE_READY, STATE_PLAYING}; // Only Ready and Playing relevant
	enum Halftime { ALL, FIRST, SECOUND, OVERTIME };

	// Getter and Setter
	bool getActiveOpponentGoalie() const { return activeOpponentGoalie; }
	void setActiveOpponentGoalie(bool value) { activeOpponentGoalie = value; }

	int getActiveOpponentMax() const { return activeOpponentMax; }
	void setActiveOpponentMax(int value) { activeOpponentMax = value; }

	int getActiveOpponentMin() const { return activeOpponentMin; }
	void setActiveOpponentMin(int value) { activeOpponentMin = value; }

	bool getActiveGoalie() const { return activeGoalie; }
	void setActiveGoalie(bool value) { activeGoalie = value; }

	int getActivePlayerMax() const { return activePlayerMax; }
	void setActivePlayerMax(int value) { activePlayerMax = value; }

	int getActivePlayerMin() const { return activePlayerMin; }
	void setActivePlayerMin(int value) { activePlayerMin = value; }

	int getDifGoalsMax() const { return difGoalsMax; }
	void setDifGoalsMax(int value) { difGoalsMax = value; }

	int getOppGoalsMax() const { return oppGoalsMax; }
	void setOppGoalsMax(int value) { oppGoalsMax = value; }

	int getDifGoalsMin() const { return difGoalsMin; }
	void setDifGoalsMin(int value) { difGoalsMin = value; }

	int getOppGoalsMin() const { return oppGoalsMin; }
	void setOppGoalsMin(int value) { oppGoalsMin = value; }

	int getOwnGoalsMax() const { return ownGoalsMax; }
	void setOwnGoalsMax(int value) { ownGoalsMax = value; }

	int getOwnGoalsMin() const { return ownGoalsMin; }
	void setOwnGoalsMin(int value) { ownGoalsMin = value; }

	Halftime getHalftime() const { return halftime; }
	void setHalftime(const Halftime &value) { halftime = value; }

	int getGameTimeRemainingMax() const { return gameTimeRemainingMax; }
	void setGameTimeRemainingMax(int value) { gameTimeRemainingMax = value; }

	int getGameTimeRemainingMin() const { return gameTimeRemainingMin; }
	void setGameTimeRemainingMin(int value) { gameTimeRemainingMin = value; }

	int getGameTimeMax() const { return gameTimeMax; }
	void setGameTimeMax(int value) { gameTimeMax = value; }

	int getGameTimeMin() const { return gameTimeMin; }
	void setGameTimeMin(int value) { gameTimeMin = value; }

	Gamemode getGamemode() const { return gamemode; }
	void setGamemode(const Gamemode &value) { gamemode = value; }
 
	bool getActiveOppGoalieToggle() const { return activeOppGoalieToggle; }
	void setActiveOppGoalieToggle(bool value) { activeOppGoalieToggle = value; }

	bool getActiveOpponentToogle() const { return activeOpponentToogle; }
	void setActiveOpponentToogle(bool value) { activeOpponentToogle = value; }

	bool getActiveGoalieToggle() const { return activeGoalieToggle; }
	void setActiveGoalieToggle(bool value) { activeGoalieToggle = value; }

	bool getActivePlayerToogle() const { return activePlayerToogle; }
	void setActivePlayerToogle(bool value) { activePlayerToogle = value; }

	bool getDifGoalsToggle() const { return difGoalsToggle; }
	void setDifGoalsToggle(bool value) { difGoalsToggle = value; }

	bool getOppGoalsToggle() const { return oppGoalsToggle; }
	void setOppGoalsToggle(bool value) { oppGoalsToggle = value; }

	bool getOwnGoalsToggle() const { return ownGoalsToggle; }
	void setOwnGoalsToggle(bool value) { ownGoalsToggle = value; }

	bool getHalftimeToggle() const { return halftimeToggle; }
	void setHalftimeToggle(bool value) { halftimeToggle = value; }

	bool getRemainingTimeToggle() const {return remainingTimeToggle;}
	void setRemainingTimeToggle(bool value) { remainingTimeToggle = value; }

	bool getTimeToggle() const { return timeToggle; }
	void setTimeToggle(bool value) { timeToggle = value; }

	bool getModeToggle() const { return modeToggle; }
	void setModeToggle(bool value) { modeToggle = value; }

	QString getName() const { return name; }
	void setName(const QString &value) { name = value; }

	QString getNote() const { return note; }
	void setNote(const QString &value) { note = value; }

	QString getFieldtype() const { return fieldtype; }
	void setFieldtype(const QString &value) { fieldtype = value; }

	FieldProperties *getFieldProperties() const { return fieldProperties; }
    void removeImpossibleFieldProperties(FieldProperties *value);

private:
	QString name;
	QString note;
	QString fieldtype;

	bool modeToggle;
	bool timeToggle;
	bool remainingTimeToggle;
	bool halftimeToggle;
	bool ownGoalsToggle;
	bool oppGoalsToggle;
	bool difGoalsToggle;
	bool activePlayerToogle;
	bool activeGoalieToggle;
	bool activeOpponentToogle;
	bool activeOppGoalieToggle;

	Gamemode gamemode;

	int gameTimeMin;
	int gameTimeMax;
	int gameTimeRemainingMin;
	int gameTimeRemainingMax;

	Halftime halftime;

	int ownGoalsMin;
	int ownGoalsMax;
	int oppGoalsMin;
	int oppGoalsMax;
	int difGoalsMin;
	int difGoalsMax;
	int activePlayerMin;
	int activePlayerMax;
	bool activeGoalie;
	int activeOpponentMin;
	int activeOpponentMax;
	bool activeOpponentGoalie;

	FieldProperties *fieldProperties;
};

#endif  // PRECONDITIONS_H
