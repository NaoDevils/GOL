#ifndef FIELDPROPERTIES_H
#define FIELDPROPERTIES_H

#include <QJsonObject>
#include <QPoint>

class FieldProperties {
public:

	FieldProperties();
	~FieldProperties();

	void write(QJsonObject &json) const;
	void read(const QJsonObject &json);

    // Getter & Setter
    QString getName() const { return name; }
    void setName(const QString &value) { name = value; }

    QPoint getCenterpoint() const { return centerpoint; }
    void setCenterpoint(const QPoint &value) { centerpoint = value; }

    int getFieldLength() const { return fieldLength; }
    void setFieldLength(int value) { fieldLength = value; }

    int getFieldWidth() const { return fieldWidth; }
    void setFieldWidth(int value) { fieldWidth = value; }

    int getLineWidth() const { return lineWidth; }
    void setLineWidth(int value) { lineWidth = value; }

    int getPenaltyMarkSize() const { return penaltyMarkSize; }
    void setPenaltyMarkSize(int value) { penaltyMarkSize = value; }

    int getPenaltyAreaLength() const { return penaltyAreaLength; }
    void setPenaltyAreaLength(int value) { penaltyAreaLength = value; }

    int getPenaltyAreaWidth() const { return penaltyAreaWidth; }
    void setPenaltyAreaWidth(int value) { penaltyAreaWidth = value; }

    int getPenaltyMarkDistance() const { return penaltyMarkDistance; }
    void setPenaltyMarkDistance(int value) { penaltyMarkDistance = value; }

    int getCenterCircleDiameter() const { return centerCircleDiameter; }
    void setCenterCircleDiameter(int value) { centerCircleDiameter = value; }

    int getBorderStripWidth() const { return borderStripWidth; }
    void setBorderStripWidth(int value) { borderStripWidth = value; }

    int getGoalRoom() const { return goalRoom; }
    void setGoalRoom(int value) { goalRoom = value; }

    int getGoalWidth() const { return goalWidth; }
    void setGoalWidth(int value) { goalWidth = value; }

    int getGoalPostSize() const { return goalPostSize; }
    void setGoalPostSize(int value) { goalPostSize = value; }

    int getTeamSize() const { return teamSize; }
    void setTeamSize(int value) { teamSize = value; }

    int getPlayerSize() const { return playerSize; }
    void setPlayerSize(int value) { playerSize = value; }

    int getBallSize() const { return ballSize; }
    void setBallSize(int value) { ballSize = value; }

    int getHalftimeDuration() const { return halftimeDuration; }
    void setHalftimeDuration(int value) { halftimeDuration = value; }

    int getFieldcenterWidth() const { return fieldcenterWidth; }
    void setFieldcenterWidth(int value) { fieldcenterWidth = value; }

    int getFieldcenterLength() const { return fieldcenterLength; }
    void setFieldcenterLength(int value) { fieldcenterLength = value; }

private:

	QString name;
	QPoint centerpoint;
	int fieldLength;
	int fieldWidth;
	int lineWidth;
	int penaltyMarkSize;
	int penaltyAreaLength;
	int penaltyAreaWidth;
	int penaltyMarkDistance;
	int centerCircleDiameter;
	int borderStripWidth;

	int goalRoom;
	int goalWidth;
	int goalPostSize;

	int teamSize;
	int playerSize;
	int ballSize;
	int halftimeDuration;

	int fieldcenterWidth;
	int fieldcenterLength;
};

#endif // FIELDPROPERTIES_H
