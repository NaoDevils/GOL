#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QGraphicsLineItem>
#include "fielditem.h"
#include "fielditembase.h"

class ArrowItem : public QGraphicsLineItem, public FieldItemBase
{
public:
	ArrowItem(FieldItem *startItem, QPointF end, QGraphicsItem *parent = 0);
	enum {Type = UserType + 10};
	int type() const Q_DECL_OVERRIDE {return Type;}

	QRectF boundingRect() const Q_DECL_OVERRIDE;
	QPainterPath shape() const;
	void update();
	void updatePosition();

	void setMyEndPoint(const QPointF &value){ myEndPoint = value;}
	QPointF getMyEndPoint() const { return myEndPoint;}
	QPointF end() const  { return myEndPoint; }

	void setMyStartItem(FieldItem *value) { myStartItem = value; }
	FieldItem *getMyStartItem() const { return myStartItem; }
	FieldItem *startItem() const { return myStartItem; }

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

private:
	FieldItem *myStartItem;
	QBrush myBrush;
	QPointF myEndPoint;
	QPolygonF arrowHead;
};
#endif // ARROWITEM_H
