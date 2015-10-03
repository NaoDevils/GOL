#ifndef FIELDSCENE_H
#define FIELDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "items/fieldproperties.h"
#include "items/fielditem.h"
#include "items/arrowitem.h"
#include "items/groupitem.h"
#include "items/field.h"
#include "items/preconditions.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

class FieldScene : public QGraphicsScene {
	Q_OBJECT

public:
	enum Mode { InsertItem, InsertLine, MoveItem };
	enum LineMode { NoLineMode, GroupMode, ArrowMode };

	explicit FieldScene(QMenu *itemMenu, QObject *parent = 0);

	QGraphicsItem *fieldItemUnderMouse;

	QList<FieldItem *> getPlayerList() const;
	void setPlayerList(const QList<FieldItem *> &value);
	QList<FieldItem *> getOpponentList() const;
	void setOpponentList(const QList<FieldItem *> &value);
	QList<FieldItem *> getBallList() const;
	void setBallList(const QList<FieldItem *> &value);
	void setMode(Mode mode);
	Mode getMode();

	void setLineMode(LineMode lineMode);
	LineMode getLineMode();

	void JSONwrite(QJsonObject &jsonObject) const;
	void JSONread(const QJsonObject &json);

	QMenu getContextMenu() const;
	void setContextMenu(const QMenu &value);
	QPointF startPos;

	FieldProperties *getFieldProperties() const;
	void setFieldProperties(FieldProperties *value);

	PreConditions *getPreConditions() const;
	void setPreConditions(PreConditions *value);

	QList<FieldProperties *> getFieldList() const;

	public slots:

	bool addPlayer();
	bool addOpponent();
	bool addBall();
    bool activateArrowMode();
    bool activateGroupMode();
	bool addGroup(FieldItem *startItem, FieldItem *endItem);
	bool addTeam();
	bool addOpponentTeam();
	bool addInitalisation();
	void removeItems();
	void removeItem(QGraphicsItem *item);

	void updateFieldPropertiesForAllItems();

	void setFieldList(const QList<FieldProperties *> &value);

	void editGroupProperties();

signals:
	void itemInserted(FieldItem *item);
	void itemSelected(FieldItem *item);
	void fieldPropertiesChanged(FieldProperties *fieldProperties);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	QMenu contextMenu;

	QMenu itemContextMenu;
	QMenu fieldContextMenu;
	QMenu groupContextMenu;
	QMenu teamContextMenu;
	QMenu opponentContextMenu;

private:
	bool isItemChange(int type);
	void updateAllItems();

	void createTeamContextMenu();
	void createFieldContextMenu();
	void createItemContextMenu();
	void createOpponentContextMenu();
	void createGroupContextMenu();

	Mode myMode;
	LineMode myLineMode;
	bool leftButtonDown;
	FieldProperties *fieldProperties;
	QList<FieldProperties *> myFieldList;
	PreConditions *preConditions;
	QPointF startPoint;
	QGraphicsLineItem *tmpline;
	QList<FieldItem *> playerList;
	QList<FieldItem *> opponentList;
	QList<FieldItem *> ballList;
	QList<GroupItem *> groupList;
	QList<ArrowItem *> arrowList;
	FieldItem *lastSelectedItem;
	FieldItem *posibleEndItem;
	bool lineCreated;
};

#endif // FIELDSCENE_H
