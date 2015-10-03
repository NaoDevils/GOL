#ifndef FIELDITEM_H
#define FIELDITEM_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsTextItem>
#include <QJsonObject>
#include <QMenu>
#include "fielditembase.h"
#include "fieldproperties.h"

class FieldItem : public QGraphicsEllipseItem, public FieldItemBase {
public:
	enum { Type = UserType + 14 };
	int type() const Q_DECL_OVERRIDE {return Type;}
	QPoint fieldPos; // Position in field coordinates
	FieldItem(FieldItemBase::ItemType itemType, FieldProperties *fieldProperties);
	void update();

	void write(QJsonObject &json) const;
	void read(const QJsonObject &json);

    // Getter & Setter
	QPointF getDestination() const { return destination; }
	void setDestination(const QPointF &value) { destination = value; }

    FieldProperties *getMyFieldProperties() const { return myFieldProperties; }
    void setMyFieldProperties(FieldProperties *value) { myFieldProperties = value; }

    QGraphicsTextItem *getItemText() const { return itemText; }

private:
	void setToBall();
	void setToPlayer();
	void setToOpponent();
	void calculateSizeClassOfField();
	void calculateDisplayCorrection();
	void updateText();

	FieldItemBase::ItemType myItemType;
	FieldProperties *myFieldProperties;
	int displayCorrection ;
	int displayCorrectionBall;
	int sizeClassOfBall;
	int sizeClassOfField;
	QGraphicsTextItem *itemText;
	QJsonObject jsonObject;
	QMenu myContextMenu;
	QPointF destination;
};

#endif // FIELDITEM_H
