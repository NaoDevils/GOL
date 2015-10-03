#include "arrowitem.h"

#include <math.h>

#include <QPen>
#include <QPainter>
#include <QDebug>

const qreal Pi = 3.14;

/**
* @brief A ArrowItem is a the item for showing the direction.
*
*
*/
ArrowItem::ArrowItem(FieldItem *startItem, QPointF end, QGraphicsItem *parent) {
	myStartItem = startItem;
	myEndPoint = end;
	itemType = path;
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setZValue(2);
	setPen(QPen(Qt::black, myStartItem->getMyFieldProperties()->getPlayerSize()/2 , Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	QLineF line(myStartItem->scenePos(), myEndPoint);
	setLine(line);
}

/**
* @brief Update for the class.
*
*
*/
void ArrowItem::update()
{
	updatePosition();
	QGraphicsLineItem::update();
}

/**
* @brief BoundingRectangle of the drawn arrow.
*
*
*/
QRectF ArrowItem::boundingRect() const {
	qreal extra = (pen().width() + 20) / 2.0; // Makes the boundingRect some pixels bigger.

	return QRectF(
		line().p1(),
		QSizeF( line().p2().x() - line().p1().x(), line().p2().y() - line().p1().y())
		).normalized().adjusted(-extra, -extra, extra, extra);
}

/**
* @brief The shape of the arrow.
*
*
*/
QPainterPath ArrowItem::shape() const {
	QPainterPath path = QGraphicsLineItem::shape();
	path.addPolygon(arrowHead);
	return path;
}

/**
* @brief Updates the positions of the start and end.
*
*
*/
void ArrowItem::updatePosition() {
	QLineF line(mapFromItem(myStartItem, 0, 0), myEndPoint);
	myStartItem->setDestination(myEndPoint);
	setLine(line);
	myStartItem->update();
}

/**
* @brief Paint the arrow
*
*
*/
void ArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
QWidget *widget) {
	QBrush myBrush(Qt::black);
	qreal arrowSize = startItem()->getMyFieldProperties()->getPlayerSize();
	painter->setPen(
	QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->setBrush(myBrush);

	double angle = ::acos(line().dx() / line().length());

	if (line().dy() >= 0) angle = (Pi * 2) - angle;

	QPointF arrowP1 = line().p2() - QPointF(sin(angle + Pi / 3) * arrowSize,cos(angle + Pi / 3) * arrowSize);
	QPointF arrowP2 = line().p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);

	QPolygonF arrowHead;
	arrowHead.clear();
	arrowHead << line().p2() << arrowP2 << arrowP1;
	painter->drawPolygon(arrowHead);
	painter->drawLine(line());
	if (isSelected()) {  // Add doted line if selected.
		painter->setPen(QPen(Qt::white, 1, Qt::DotLine));
		QLineF myLine = line();
		myLine.translate(0, 4.0);
		painter->drawLine(myLine);
		myLine.translate(0, -8.0);
		painter->drawLine(myLine);
		painter->drawPolygon(arrowHead);
	}
}