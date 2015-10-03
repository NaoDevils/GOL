#ifndef FIELDLISTITEM_H
#define FIELDLISTITEM_H

#include <QListWidgetItem>
#include "fieldproperties.h"

class FieldListItem : public QObject, public QListWidgetItem
{
	Q_OBJECT
public:
	FieldListItem(FieldProperties *fieldProperties);
	~FieldListItem();

    // Getter & Setter
    FieldProperties *getMyFieldProperties() const {return myFieldProperties;}
    void setMyFieldProperties(FieldProperties *value) {myFieldProperties = value;}

private:

	FieldProperties *myFieldProperties;
};

#endif // FIELDLISTITEM_H
