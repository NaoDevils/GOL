#include "fielditem.h"
#include <QPainter>
#include <QDebug>
#include "math.h"

/**
* @brief The player, opponent or ball visible on the field.
*
* Only one basic FieldItem for player, opponent or ball, because it is easier for using in mixed lists.
*/
FieldItem::FieldItem(FieldItemBase::ItemType itemType,
FieldProperties *fieldProperties) {
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	myFieldProperties = fieldProperties;
	displayCorrection = 1;
	displayCorrectionBall = 1;
	myItemType = itemType;
	fieldPos.setX(scenePos().x());
	fieldPos.setY(scenePos().y());
	destination = fieldPos;
	setZValue(3);
	if (myItemType == FieldItemBase::ball) { setToBall(); }
	if (myItemType == FieldItemBase::opponent) { setToOpponent(); }
	if (myItemType == FieldItemBase::player) { setToPlayer(); }
	itemText = new QGraphicsTextItem(name);
}

/**
* @brief Updates the item, for example the save positions.
*
*/
void FieldItem::update() {
	fieldPos.setX(scenePos().x());
	fieldPos.setY(scenePos().y());
	calculateSizeClassOfField();
	calculateDisplayCorrection();

	if (itemType == FieldItemBase::ball) { // a ball is smaller.
		setRect(0 - (myFieldProperties->getBallSize() / 2),
			0 - (myFieldProperties->getBallSize() / 2),
			myFieldProperties->getBallSize() * displayCorrectionBall,
			myFieldProperties->getBallSize() * displayCorrectionBall
			);
	} else { // player or opponent
		setRect(
			0 - (myFieldProperties->getPlayerSize() / 2),
			0 - (myFieldProperties->getPlayerSize() / 2),
			myFieldProperties->getPlayerSize() * displayCorrection,
			myFieldProperties->getPlayerSize() * displayCorrection
			);
		}
	if(itemType == player || itemType == opponent) {updateText();} // Only players and opponents have text to display.
	QGraphicsItem::update();
}

/**
* @brief Redraw the name of a FieldItem in the sceneview.
*
*/
void FieldItem::updateText(){
	itemText->setDefaultTextColor(Qt::gray);
	itemText->setPlainText(name);
	itemText->setPos(scenePos());
	itemText->setZValue(zValue()+1);
	itemText->setScale(2.5);
	QPainter painter;
	painter.drawText(scenePos(),name);
}


/**
* @brief Sets the item to a ball.
*
*/
void FieldItem::setToBall() {
	setBrush(QColor(255, 165, 0));  // Orange
	setZValue(5);
	name = "Ball";
	itemType = ball;
	update();
	setRect(
		myFieldProperties->getCenterpoint().x() - (myFieldProperties->getBallSize() / 2),
		myFieldProperties->getCenterpoint().y() - (myFieldProperties->getBallSize() / 2),
		myFieldProperties->getBallSize() * displayCorrectionBall,
		myFieldProperties->getBallSize() * displayCorrectionBall
		);
}

/**
* @brief Sets the item to a ball.
*
*/
void FieldItem::setToPlayer() {
	setBrush(Qt::blue);
	itemType = player;
	name = "P";
	setRect(
		myFieldProperties->getCenterpoint().x() - myFieldProperties->getPlayerSize() /2,
		myFieldProperties->getCenterpoint().y() - myFieldProperties->getPlayerSize() /2,
		myFieldProperties->getPlayerSize() * displayCorrection,
		myFieldProperties->getPlayerSize() * displayCorrection
		);
}

/**
* @brief Sets the item to an opponent.
*
*/
void FieldItem::setToOpponent() {
	setBrush(Qt::red);
	itemType = opponent;
	name = "O";
	itemText = new QGraphicsTextItem(name);
	setRect(
		myFieldProperties->getCenterpoint().x() - myFieldProperties->getPlayerSize() / 2,
		myFieldProperties->getCenterpoint().y() - myFieldProperties->getPlayerSize() / 2,
		myFieldProperties->getPlayerSize() * displayCorrection,
		myFieldProperties->getPlayerSize() * displayCorrection);
}

/**
* @brief Calculates the size of the field and the size of the ball, for display correction.
*
*/
void FieldItem::calculateSizeClassOfField() {
	sizeClassOfField = log10((float)myFieldProperties->getFieldLength()) + 1;
	sizeClassOfBall = log10((float)myFieldProperties->getBallSize()) + 1;
}

/**
* @brief If the ball is to small on the field the items should be drawn bigger.
*
*/
void FieldItem::calculateDisplayCorrection() {
	displayCorrection = abs(sizeClassOfField - sizeClassOfBall);

	if (sizeClassOfField != 3) { displayCorrectionBall = (displayCorrection + 2) * 2; }

	if (displayCorrection == 0) { displayCorrection = 1; }
	if (displayCorrectionBall == 0) { displayCorrectionBall = 1; }
}

/**
* @brief Converts the FieldItem into JSON.
*
*/
void FieldItem::write(QJsonObject &json) const {
	json["name"] = name;
	json["x"] = (int)(scenePos().x());
	json["y"] = (int)(- scenePos().y());
	if (itemType == player || itemType == ball) {
		json["xn"] = (int)(destination.x());
		json["yn"] = (int)(- destination.y());
	}
}

/**
* @brief Creates a FieldItem from JSON.
*
*/
void FieldItem::read(const QJsonObject &json) {
	name = json["name"].toString();
	fieldPos.setX(json["x"].toInt());
	fieldPos.setY(-(json["y"].toInt()));
	if (itemType == player || itemType == ball) {
		destination.setX(json["xn"].toInt());
		destination.setY(-(json["yn"].toInt()));
	}
}
