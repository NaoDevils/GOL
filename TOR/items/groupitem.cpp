#include "groupitem.h"

#include <QPainter>
#include <QDebug>
#include "fieldproperties.h"

/**
* @brief The Item for saving a group.
*
*
*/
GroupItem::GroupItem(QGraphicsItem *parent) {
	setFlag(ItemIsSelectable);
	setZValue(1);
	FieldProperties *fieldProperties = new FieldProperties;
	myStartItem = new FieldItem(FieldItemBase::misc, fieldProperties);
	myEndItem = new FieldItem(FieldItemBase::misc, fieldProperties);
	name = "gr" + name;
	setPen(QPen(Qt::black, 10, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
	QLineF line(myStartItem->scenePos(), myEndItem->scenePos());
	setLine(line);
	itemType = grouped;
}

/**
* @brief The Item for saving a group, with both members of a group.
*
*
*/
GroupItem::GroupItem(FieldItem *startItem, FieldItem *endItem, QGraphicsItem *parent) : QGraphicsLineItem(parent) {
	name = "gr" + name;
	myStartItem = startItem;
	myEndItem = endItem;
	setFlag(ItemIsSelectable);
	setZValue(2);
	setPen(QPen(Qt::black, 15, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
	QLineF line(myStartItem->scenePos(), myEndItem->scenePos());
	setLine(line);
	xDistance = myStartItem->scenePos().x() - myEndItem->scenePos().x();
	yDistance = myStartItem->scenePos().y() - myEndItem->scenePos().y();
}

/**
* @brief The bounding rectangle of the group-line.
*
*
*/
QRectF GroupItem::boundingRect() const {
	qreal extra = (pen().width() + 20) / 2.0;
	return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(), line().p2().y() - line().p1().y())) .normalized() .adjusted(-extra, -extra, extra, extra);
}

/**
* @brief Redraw line if something has changed.
*
*
*/
void GroupItem::updatePosition() {
	QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
	setLine(line);
}

/**
* @brief Writes the object to JSON.
*
*
*/
void GroupItem::write(QJsonObject &json) const {
	json["name"] = name;
	json["leader"] = myStartItem->getName();
	json["follower"] = myEndItem->getName();
	json["dX"] = xDistance;
	json["dY"] = -yDistance;
}

/**
* @brief Reads a object from JSON.
*
*
*/
void GroupItem::read(const QJsonObject &json, QList<FieldItem *> playerList,
QList<FieldItem *> opponentList,
QList<FieldItem *> ballList) {
	name = json["name"].toString();
	QString leaderName = json["leader"].toString();
	QString followerName = json["follower"].toString();

	// Find the members of the group in the lists.
	for (int index = 0; index < ballList.size(); ++index) {
		if (leaderName == ballList[index]->getName()) setMyStartItem(ballList[index]);
		if (followerName == ballList[index]->getName()) setMyEndItem(ballList[index]);
	};
	for (int index = 0; index < playerList.size(); ++index) {
		if (leaderName == playerList[index]->getName()) setMyStartItem(playerList[index]);
		if (followerName == playerList[index]->getName()) setMyEndItem(playerList[index]);
	};
	for (int index = 0; index < opponentList.size(); ++index) {
		if (leaderName == opponentList[index]->getName()) setMyStartItem(opponentList[index]);
		if (followerName == opponentList[index]->getName()) setMyEndItem(opponentList[index]);
	};

	xDistance = json["dX"].toInt();
	yDistance = -(json["dY"].toInt());
}

/**
* @brief Painter for the group.
*
*
*/
void GroupItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
QWidget *widget) {
	painter->setPen(
	QPen(Qt::black, 10, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(line());
	if (isSelected()) {
		painter->setPen(QPen(Qt::white, 1, Qt::DotLine));
		QLineF myLine = line();
		myLine.translate(0, 5.0);
		painter->drawLine(myLine);
		myLine.translate(0, -9.0);
		painter->drawLine(myLine);
	}
}