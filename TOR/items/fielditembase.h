#ifndef FIELDITEMBASE_H
#define FIELDITEMBASE_H

#include <QString>

class FieldItemBase
{
public:
	FieldItemBase();

	enum ItemType { player, opponent, ball, grouped, path, misc };

//	void redraw();

    ItemType getItemType() const { return itemType; }
    void setItemType(const ItemType &value) { itemType = value; }

    QString getName() const { return name; }
    void setName(const QString &value) { name = value; }

protected:

	ItemType itemType;
	QString name;
};
#endif // FIELDITEMBASE_H
