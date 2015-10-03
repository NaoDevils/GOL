#include "field.h"
#include <QDebug>
#include <QGraphicsItem>

/**
* @brief Item Field for showing the lines on the screen.
*
*/
Field::Field(FieldProperties *fieldProperties, QGraphicsItem *parent) {
	myFieldProperties = fieldProperties;
	setZValue(-1);
	setFlag(ItemIsMovable, false);
	setFlag(ItemIsFocusable, false);
	setFlag(ItemIsSelectable, false);
}

/**
* @brief Gets the boundingRect of the field.
*
*/
QRectF Field::getBoundingRect() const
{
	return boundingRect();
}

/**
* @brief Defines the bounding rectangle of the field.
*
*/
QRectF Field::boundingRect() const {
	return QRectF(
		-myFieldProperties->getFieldLength() / 2 - myFieldProperties->getBorderStripWidth(),
		-myFieldProperties->getFieldWidth() / 2 - myFieldProperties->getBorderStripWidth(),
		2 * myFieldProperties->getBorderStripWidth() + myFieldProperties->getFieldLength(),
		2 * myFieldProperties->getBorderStripWidth() + myFieldProperties->getFieldWidth()
		);
}

/**
* @brief Paints the field.
*
*/
void Field::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
QWidget *widget) {
	QBrush greenBrush(Qt::darkGreen);
	QBrush yellowBrush(Qt::yellow);
	QBrush whiteBrush(Qt::white);
	QBrush greenBrush2(QColor(0, 225, 0, 60));
	QPen blackPen(Qt::black);
	QPen yellowPen(Qt::yellow);
	QPen whitePen(Qt::white);
	QPen greenPen(Qt::green);
	QPen greenPen2(QColor(0, 225, 0, 60));

	greenPen2.setWidth(60);
	greenPen2.setJoinStyle(Qt::MiterJoin);

	blackPen.setWidth(2);
	whitePen.setWidth(myFieldProperties->getLineWidth());
	greenPen.setWidth(1);
	yellowPen.setWidth(0);

	//    field boarders
	painter->setPen(blackPen);
	painter->setBrush(greenBrush);
	QRectF fieldBoarders = boundingRect();
	setRect(fieldBoarders);
	painter->drawRect(fieldBoarders);

	// field lines
	QRectF fieldLines(
		-myFieldProperties->getFieldLength() / 2,
		-myFieldProperties->getFieldWidth() / 2,
		myFieldProperties->getFieldLength(),
		myFieldProperties->getFieldWidth()
		);
	painter->setPen(whitePen);
	painter->drawRect(fieldLines);

	// center  circle
	QRectF centerCircle(
		-myFieldProperties->getCenterCircleDiameter() / 2,
		-myFieldProperties->getCenterCircleDiameter() / 2,
		myFieldProperties->getCenterCircleDiameter(),
		myFieldProperties->getCenterCircleDiameter()
		);
	painter->drawEllipse(centerCircle);

	// middle line
	QRectF middleLine(
		0,
		-myFieldProperties->getFieldWidth() / 2,
		myFieldProperties->getLineWidth() / 4,
		myFieldProperties->getFieldWidth()
		);
	painter->setPen(whitePen);
	painter->drawRect(middleLine);

	// center point
	QRectF centerPoint(
		-myFieldProperties->getPenaltyMarkSize() / 2,
		-myFieldProperties->getPenaltyMarkSize() / 2,
		myFieldProperties->getPenaltyMarkSize(),
		myFieldProperties->getPenaltyMarkSize()
		);
	painter->drawEllipse(centerPoint);

	// penalty area

	QRectF penaltyAreaLeft(
		-myFieldProperties->getFieldLength() / 2,
		-myFieldProperties->getPenaltyAreaWidth() / 2,
		myFieldProperties->getPenaltyAreaLength(),
		myFieldProperties->getPenaltyAreaWidth()
		);
	painter->drawRect(penaltyAreaLeft);

	QRectF penaltyAreaRight(
		myFieldProperties->getFieldLength() / 2,
		-myFieldProperties->getPenaltyAreaWidth() / 2,
		-myFieldProperties->getPenaltyAreaLength(),
		myFieldProperties->getPenaltyAreaWidth()
		);
	painter->drawRect(penaltyAreaRight);

	// penalty mark

	painter->setBrush(whiteBrush);
	QRectF penaltyMarkLeft(
		-myFieldProperties->getFieldLength() / 2 + myFieldProperties->getPenaltyMarkDistance(), -myFieldProperties->getPenaltyMarkSize() / 2,
		myFieldProperties->getPenaltyMarkSize(), myFieldProperties->getPenaltyMarkSize()
		);
	painter->drawEllipse(penaltyMarkLeft);

	QRectF penaltyMarkRight(
		myFieldProperties->getFieldLength() / 2 - myFieldProperties->getPenaltyMarkDistance(), -myFieldProperties->getPenaltyMarkSize() / 2,
		myFieldProperties->getPenaltyMarkSize(), myFieldProperties->getPenaltyMarkSize()
		);
	painter->drawEllipse(penaltyMarkRight);

	// goals

	painter->setPen(yellowPen);
	painter->setBrush(yellowBrush);
	QRectF goalLeft( -myFieldProperties->getFieldLength() / 2, -myFieldProperties->getGoalWidth() / 2 - myFieldProperties->getGoalPostSize(),
		-myFieldProperties->getGoalRoom(), myFieldProperties->getGoalWidth() + 2 * myFieldProperties->getGoalPostSize()
		);
	painter->drawRect(goalLeft);

	QRectF goalRight( myFieldProperties->getFieldLength() / 2, -myFieldProperties->getGoalWidth() / 2 - myFieldProperties->getGoalPostSize(),
		myFieldProperties->getGoalRoom(), myFieldProperties->getGoalWidth() + 2 * myFieldProperties->getGoalPostSize()
		);
	painter->drawRect(goalRight);

	// tactic direction
	int arrowHeadSize = myFieldProperties->getCenterCircleDiameter()/2;
	int arrowPosition = myFieldProperties->getFieldLength() / 2 - myFieldProperties->getPenaltyMarkDistance()- arrowHeadSize;

	QLineF tacticline(
		-myFieldProperties->getFieldLength() / 2 + 	myFieldProperties->getPenaltyMarkDistance() + arrowHeadSize + greenPen2.width(), 0,
		myFieldProperties->getFieldLength() / 2 - myFieldProperties->getPenaltyMarkDistance() - arrowHeadSize*2 - greenPen2.width(), 0
		);
	painter->setPen(greenPen2);
	painter->setBrush(Qt::transparent);
	painter->drawLine(tacticline);

	QPolygonF arrowHead;
	arrowHead
		<< QPointF(arrowPosition, 0)
		<< QPointF(arrowPosition - arrowHeadSize, arrowHeadSize/2)
		<< QPointF(arrowPosition - arrowHeadSize, - arrowHeadSize/2);

	painter->drawPolygon(arrowHead);
}
