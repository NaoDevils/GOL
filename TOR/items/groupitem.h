#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QGraphicsLineItem>
#include <QJsonObject>
#include "fielditem.h"
#include "fielditembase.h"

QT_BEGIN_NAMESPACE
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class GroupItem : public QGraphicsLineItem, public FieldItemBase
{
public:
	enum {Type = UserType + 11};

	GroupItem(QGraphicsItem *parent = 0);
	GroupItem(FieldItem *startItem, FieldItem *endItem, QGraphicsItem *parent = 0);

	int type() const Q_DECL_OVERRIDE {return Type;}
	QRectF boundingRect() const Q_DECL_OVERRIDE;
	FieldItem *startItem() const {return myStartItem;}
	FieldItem *endItem() const  {return myEndItem;}

	void updatePosition();

	void write(QJsonObject &json) const;
	void read(const QJsonObject &json, QList<FieldItem*> playerList, QList<FieldItem*> opponentList, QList<FieldItem*> ballList);

	// Getter & Setter

	FieldItem *getMyStartItem() const { return myStartItem; }
	void setMyStartItem(FieldItem *value) { myStartItem = value; }

	FieldItem *getMyEndItem() const { return myEndItem; }
	void setMyEndItem(FieldItem *value) { myEndItem = value; }

	int getXDistance() const { return xDistance; }
	void setXDistance(int value) { xDistance = value; }

	int getYDistance() const { return yDistance; }
	void setYDistance(int value) { yDistance = value; }

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

private:
	FieldItem *myStartItem;
	FieldItem *myEndItem;
	QBrush myBrush;
	int xDistance;
	int yDistance;
	QJsonObject jsonObject;
	QList<FieldItem *> playerList;
	QList<FieldItem *> opponentList;
	QList<FieldItem *> ballList;
};

#endif // GROUPITEM_H
