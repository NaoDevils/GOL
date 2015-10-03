#ifndef FIELD_H
#define FIELD_H

#include <QPainter>
#include <QGraphicsRectItem>
#include "fielditem.h"
#include "fieldproperties.h"

class Field : public QGraphicsRectItem {
public:
	enum {Type = UserType + 24};
	Field(FieldProperties *fieldProperties, QGraphicsItem *parent = NULL);
    FieldProperties *getMyFieldProperties() const
    {
        return myFieldProperties;
    }
	int type() const Q_DECL_OVERRIDE {return Type;}
	QRectF getBoundingRect() const;
    void setMyFieldProperties(FieldProperties *value)
    {
        myFieldProperties = value;
    }

protected:
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	FieldProperties *myFieldProperties;
};

#endif // FIELD_H
