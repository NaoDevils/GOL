#include "fieldscene.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include "ui\grouppropertiesdialog.h"
#include "ui\numberdialog.h"
#include "items\groupitem.h"
#include <QGraphicsItem>

/**
* @brief Class FieldScene is the structure where every object of a TacticBoard is managed.
*
*/

FieldScene::FieldScene(QMenu *itemMenu, QObject *parent)
: QGraphicsScene(parent) {
	myMode = MoveItem;
	myLineMode = NoLineMode;
	lineCreated = false;
	preConditions = new PreConditions();
	fieldProperties = preConditions->getFieldProperties();
}

/**
* @brief Function for adding a player to the scene.
*
*
*/
bool FieldScene::addPlayer() {
	if (playerList.count() >= fieldProperties->getTeamSize()) { // Team is full.
		QMessageBox::information( NULL, "Regelverstoß", "Objekt nicht hinzugefügt.\nZuviele Spieler im Spiel.");
		return 0;
	} else {
		setMode(FieldScene::MoveItem);
		FieldItem *player = new FieldItem(FieldItemBase::player, fieldProperties);
		addItem(player);
		playerList.append(player);

		NumberDialog *d = new NumberDialog(player, playerList, fieldProperties);
		d->show();
		addItem(player->getItemText());
		return 1;
	}
}

/**
* @brief Function for adding a opponent to the scene.
*
*
*/
bool FieldScene::addOpponent() {
	if (opponentList.count() >= fieldProperties->getTeamSize()) { // Team is full.
		QMessageBox::information( NULL, "Regelverstoß", "Objekt nicht hinzugefügt.\nZuviele Gegner im Spiel."); return 0;
	} else {
		setMode(FieldScene::MoveItem);
		FieldItem *opponent = new FieldItem(FieldItemBase::opponent, fieldProperties);
		addItem(opponent);
		opponentList.append(opponent);

		NumberDialog *d = new NumberDialog(opponent, opponentList, fieldProperties);
		d->show();
		addItem(opponent->getItemText());
		return 1;
	}
}

/**
* @brief Function for adding the ball to the scene.
*
*
*/
bool FieldScene::addBall() {
	if (ballList.count() >= 1) { // Only one ball is allowed.
		QMessageBox::information( NULL, "Regelverstoß", "Objekt nicht hinzugefügt.\nZuviele Bälle im Spiel."); return 0;
	} else {
		setMode(FieldScene::MoveItem);
		FieldItem *ball = new FieldItem(FieldItemBase::ball, fieldProperties);
		addItem(ball);
		ballList.append(ball);
		return 1;
	}
}

/**
* @brief Function for activating the painting of a arrow on the field.
*
*
*/
bool FieldScene::activateArrowMode() {
	myMode = InsertLine;
	myLineMode = ArrowMode;
	return 0;
}

/**
* @brief Function for activating the option for grouping of objects.
*
*
*/
bool FieldScene::activateGroupMode() {
	myMode = InsertLine;
	myLineMode = GroupMode;
	return 0;
}

/**
* @brief Function for adding a group to the scene.
*
*
*/
bool FieldScene::addGroup(FieldItem *startItem, FieldItem *endItem) {
	GroupItem *groupItem = new GroupItem(lastSelectedItem, posibleEndItem);
	addItem(groupItem);
	groupList.append(groupItem);

	GroupPropertiesDialog *d =	new GroupPropertiesDialog(groupItem, fieldProperties);
	d->show();
	return 0;
}

/**
* @brief Function for adding a team to the scene.
*
*
*/
bool FieldScene::addTeam() {
	bool teamAdded = false;
	if (playerList.isEmpty()) { // Team can only be added if list of players is empty.
		QDir dir(QCoreApplication::applicationDirPath());
		if (!dir.cd("config")) { // Check if config-folder is there.
			qWarning("Kann den Ordner nicht finden.");
		} else {
			QFile file(dir.filePath("positionbyrules.rule"));
			if (!file.open(QIODevice::ReadOnly)) { // Check if rule-file can be opened.
				qWarning("Datei kann nicht gelesen werden.");
			} else { // Read team as scene from JSON.
				QByteArray saveData = file.readAll();
				QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
				QJsonArray jsonPositionsArray = loadDoc.array();

				for (int index = 0; index < jsonPositionsArray.size(); ++index) {
					QJsonObject jsonPositionsObject = jsonPositionsArray[index].toObject();
					QJsonObject jsonPreConditions = jsonPositionsObject["PreConditions"].toObject();
					PreConditions *tempPreConditions = new PreConditions();
					tempPreConditions->read(jsonPreConditions);
					if (tempPreConditions->getFieldtype() == preConditions->getFieldtype()) {
						teamAdded = true;
						QJsonArray jsonArrayPlayer;
						jsonArrayPlayer = jsonPositionsObject["Players"].toArray();
						for (int index = 0; index < jsonArrayPlayer.size(); ++index) {
							QJsonObject jsonPlayer = jsonArrayPlayer[index].toObject();
							FieldItem *player =
							new FieldItem(FieldItemBase::player, fieldProperties);
							player->read(jsonPlayer);
							player->setPos(player->fieldPos);
							addItem(player);
							addItem(player->getItemText());
							playerList.append(player);
							playerList[0]->setSelected(true);
						}
					}
				}
			}
		}
	}
	if (teamAdded == false) { QMessageBox::information(NULL, "Fehler", "Konnte keine passende Formation laden."); }
	return teamAdded;
}

/**
* @brief Function for adding a opponent team to the scene.
*
*
*/
bool FieldScene::addOpponentTeam() {
	bool teamAdded = false;
	if (opponentList.isEmpty()) {
		QDir dir(QCoreApplication::applicationDirPath());
		if (!dir.cd("config")) { // Check if config-folder is there.
			qWarning("Kann den Ordner nicht finden.");
		} else {
			QFile file(dir.filePath("positionbyrules.rule"));
			if (!file.open(QIODevice::ReadOnly)) { // Check if rule-file can be opened.
				qWarning("Datei kann nicht gelesen werden.");
			} else { // Read team as scene from JSON.
				QByteArray saveData = file.readAll();
				QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
				QJsonArray jsonPositionsArray = loadDoc.array();

				for (int index = 0; index < jsonPositionsArray.size(); ++index) {
					QJsonObject jsonPositionsObject = jsonPositionsArray[index].toObject();
					QJsonObject jsonPreConditions = jsonPositionsObject["PreConditions"].toObject();
					PreConditions *tempPreConditions = new PreConditions();
					tempPreConditions->read(jsonPreConditions);
					if (tempPreConditions->getFieldtype() == preConditions->getFieldtype()) {
						teamAdded = true;
						QJsonArray jsonArrayOpponent;
						jsonArrayOpponent = jsonPositionsObject["Opponents"].toArray();
						for (int index = 0; index < jsonArrayOpponent.size(); ++index) {
							QJsonObject jsonOpponent = jsonArrayOpponent[index].toObject();
							FieldItem *opponent = new FieldItem(FieldItemBase::opponent, fieldProperties);
							opponent->read(jsonOpponent);
							opponent->setPos(opponent->fieldPos);
							addItem(opponent);
							addItem(opponent->getItemText());
							opponentList.append(opponent);
							opponentList[0]->setSelected(true);
						}
					}
				}
			}
			if (teamAdded == false) { QMessageBox::information(NULL, "Fehler", "Konnte keine passende Formation laden."); }
		}
	}
	return teamAdded;
}

/**
* @brief Function for adding a team, opponent-team and ball to the scene.
*
*
*/
bool FieldScene::addInitalisation() {
	addTeam();
	addOpponentTeam();
	addBall();
	return 1;
}

/**
* @brief Function for removing select items from the scene.
*
*
*/
void FieldScene::removeItems() {
	foreach (QGraphicsItem *item, selectedItems()) {
		removeItem(item);
	}
}

/**
* @brief Function for removing the special item
* @param item The item which should be removed.
*
*/
void FieldScene::removeItem(QGraphicsItem *item) {
	// Remove the item from every group
	foreach (GroupItem *group, groupList) {
		if (group->startItem() == qgraphicsitem_cast<FieldItem *>(item)
			|| group->endItem() == qgraphicsitem_cast<FieldItem *>(item)) {
			groupList.removeAll(group);
			removeItem(group);
		}
	}
	// Remove the item from every arrow
	foreach (ArrowItem *arrow, arrowList) {
		if (arrow->startItem() == qgraphicsitem_cast<FieldItem *>(item)) {
			arrowList.removeAll(arrow);
			delete arrow;
		}
	}

	// Remove the item from list.
	playerList.removeAll(qgraphicsitem_cast<FieldItem *>(item));
	opponentList.removeAll(qgraphicsitem_cast<FieldItem *>(item));
	ballList.removeAll(qgraphicsitem_cast<FieldItem *>(item));
	groupList.removeAll(qgraphicsitem_cast<GroupItem *>(item));
	arrowList.removeAll(qgraphicsitem_cast<ArrowItem *>(item));

	if(item->type() == 65550) QGraphicsScene::removeItem(qgraphicsitem_cast<FieldItem *>(item)->getItemText()); // If item as a show text, remove it.

	QGraphicsScene::removeItem(item); // Item can now be safety removed.
	delete item;
}

/**
* @brief If the FieldProperties have changed, every item with FieldProperties has to be updated.
*
*
*/
void FieldScene::updateFieldPropertiesForAllItems() {
	fieldProperties = preConditions->getFieldProperties();
	// Give every item the new FieldProperties
	foreach (QGraphicsItem *item, items()) {
		switch (item->type()) {
		case 65550:  // FieldItem
			qgraphicsitem_cast<FieldItem *>(item) ->setMyFieldProperties(fieldProperties);
			break;
		case 65560:  // Field
			qgraphicsitem_cast<Field *>(item) ->setMyFieldProperties(fieldProperties);
			item->update();
			break;
		case 65546:  // ArrowItem
			break;
		case 65547:  // GroupItem
			break;
		case 65540:  // FieldItemBase
			break;
		case 8:  // TextItem
			break;
		default:
			qDebug() << "Unbekanntes Item" << item->type();
		}
	}
	setSceneRect(itemsBoundingRect());
}

/**
* @brief Writing the objects to JSON
*
*
*/
void FieldScene::JSONwrite(QJsonObject &jsonObject) const {
	QJsonObject jsonPreConditions;
	QJsonArray jsonArrayPlayer;
	QJsonArray jsonArrayOpponent;
	QJsonArray jsonArrayGroup;
	QJsonObject jsonObjectPlayer;
	QJsonObject jsonObjectOpponent;
	QJsonObject jsonObjectBall;
	QJsonObject jsonObjectGroup;

	// Write every player which is known.
	for (int index = 0; index < playerList.size(); ++index) {
		playerList[index]->write(jsonObjectPlayer);
		jsonArrayPlayer.append(jsonObjectPlayer);
	}

	// Write every opponent which is known.
	for (int index = 0; index < opponentList.size(); ++index) {
		opponentList[index]->write(jsonObjectOpponent);
		jsonArrayOpponent.append(jsonObjectOpponent);
	}

	// Write every group which is known.
	for (int index = 0; index < groupList.size(); ++index) {
		groupList[index]->write(jsonObjectGroup);
		jsonArrayGroup.append(jsonObjectGroup);
	}

	// Write every ball which is known.
	for (int index = 0; index < ballList.size(); ++index) {
		ballList[index]->write(jsonObjectBall);
	}

	preConditions->write(jsonPreConditions);

	jsonObject["PreConditions"] = jsonPreConditions;
	jsonObject["Ball"] = jsonObjectBall;
	jsonObject["Opponents"] = jsonArrayOpponent;
	jsonObject["Players"] = jsonArrayPlayer;
	jsonObject["Groups"] = jsonArrayGroup;
}

/**
* @brief JSON to objects
*
*
*/
void FieldScene::JSONread(const QJsonObject &json) {
	// Read PreConditions
	if (json["PreConditions"].isObject()) {
		QJsonObject jsonObjectPreConditions;
		jsonObjectPreConditions = json["PreConditions"].toObject();
		preConditions->read(jsonObjectPreConditions);

		// Set Scene FieldProperties
		foreach (FieldProperties *fieldPropertiesItem, myFieldList) {
			if (fieldPropertiesItem->getName() == preConditions->getFieldtype()) {
				fieldProperties = fieldPropertiesItem;
				preConditions->removeImpossibleFieldProperties(fieldProperties);
			}
		}
	}

	// Read ball
	if (json["Ball"].isObject()) {
		QJsonObject jsonObjectBall;
		jsonObjectBall = json["Ball"].toObject();
		FieldItem *ball = new FieldItem(FieldItemBase::ball, fieldProperties);
		ball->read(jsonObjectBall);
		ball->setPos(ball->fieldPos);
		addItem(ball);
		if (ball->fieldPos != ball->getDestination()) {
			ArrowItem *arrow = new ArrowItem(ball, ball->getDestination());
			arrowList.append(arrow);
			addItem(arrow);
		}
		ballList.append(ball);
	}

	// Read players
	if (json["Players"].isArray()) {
		QJsonArray jsonArrayPlayer;
		jsonArrayPlayer = json["Players"].toArray();
		for (int index = 0; index < jsonArrayPlayer.size(); ++index) {
			QJsonObject jsonPlayer = jsonArrayPlayer[index].toObject();
			FieldItem *player = new FieldItem(FieldItemBase::player, fieldProperties);
			player->read(jsonPlayer);
			player->setPos(player->fieldPos);
			addItem(player);
			addItem(player->getItemText());
			if (player->fieldPos != player->getDestination()) {
				ArrowItem *arrow = new ArrowItem(player, player->getDestination());
				arrowList.append(arrow);
				addItem(arrow);
			}

			playerList.append(player);
		}
	}

	// Read opponents
	if (json["Opponents"].isArray()) {
		QJsonArray jsonArrayOpponent;
		jsonArrayOpponent = json["Opponents"].toArray();
		for (int index = 0; index < jsonArrayOpponent.size(); ++index) {
			QJsonObject jsonOpponent = jsonArrayOpponent[index].toObject();
			FieldItem *opponent =
			new FieldItem(FieldItemBase::opponent, fieldProperties);
			opponent->read(jsonOpponent);
			opponent->setPos(opponent->fieldPos);
			addItem(opponent);
			addItem(opponent->getItemText());
			opponentList.append(opponent);
		}
	}

	// Read Groups
	if (json["Groups"].isArray()) {
		QJsonArray jsonArrayGroup;
		jsonArrayGroup = json["Groups"].toArray();

		for (int index = 0; index < jsonArrayGroup.size(); ++index) {
			QJsonObject jsonGroup = jsonArrayGroup[index].toObject();
			GroupItem *group = new GroupItem();
			group->read(jsonGroup, playerList, opponentList, ballList);
			addItem(group);
			groupList.append(group);
		}
	}
}

/**
* @brief Handling the event if the mouse is pressed.
*
*
*/

void FieldScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	if (mouseEvent->button() != Qt::LeftButton) return; // Right button, do nothing.
	if (!itemAt(mouseEvent->scenePos(), QTransform())) return; // If no item is under the mouse, do nothing.
	QGraphicsItem *itemUnderMouse = itemAt(mouseEvent->scenePos(), QTransform());

	if (itemUnderMouse->type() != 65560) { // Not the field.
		if (itemUnderMouse->type() == 65550) { // FieldItem
			lastSelectedItem = qgraphicsitem_cast<FieldItem *>(itemUnderMouse);
		}
		if(itemUnderMouse->type() == 65546){ // ArrowItem
			lastSelectedItem  = qgraphicsitem_cast<ArrowItem *>(itemUnderMouse)->getMyStartItem();
			arrowList.removeAll(qgraphicsitem_cast<ArrowItem *>(itemUnderMouse)); // Delete Arrow because of redrawing.
			removeItem(itemUnderMouse);
			myMode = InsertLine;
			myLineMode = ArrowMode;
		}
	}
	switch (myMode) {
	case InsertLine:
		tmpline = new QGraphicsLineItem(
		QLineF(lastSelectedItem->scenePos(), mouseEvent->scenePos()));

		switch (myLineMode) {
		case ArrowMode:
			tmpline->setPen(
			QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			break;
		case GroupMode:
			tmpline->setPen(
			QPen(Qt::black, 10, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
			break;
		default:
			;
		}
		addItem(tmpline);
		lineCreated = true;
		break;
	default:
		;
	}

	QGraphicsScene::mousePressEvent(mouseEvent);
}

/**
* @brief Handling move event for the mouse.
* If the scene is in linemode draw a line.
*
*/
void FieldScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	if (myMode == InsertLine && tmpline != 0 && lineCreated == true) {
		QLineF newLine(lastSelectedItem->scenePos(), mouseEvent->scenePos());
		tmpline->setLine(newLine);
	}
	if (myMode == MoveItem) {
		QGraphicsScene::mouseMoveEvent(mouseEvent);
		updateAllItems();
	}
}

/**
* @brief Handling the event when the mouse is released.
*
*
*/
void FieldScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	QGraphicsItem *itemUnderMouse = itemAt(mouseEvent->scenePos(), QTransform());

	if (itemUnderMouse) {
		if (tmpline != 0
			&& myMode == InsertLine
			&& lineCreated == true) {  // A line is created
			lineCreated = false;
			myMode = MoveItem;
			switch (myLineMode) {
			case GroupMode:
				if (itemUnderMouse->type() == 65550) {
					posibleEndItem = qgraphicsitem_cast<FieldItem *>(itemUnderMouse);
					// One member is a player
					if ((posibleEndItem->getItemType() == FieldItemBase::player
						|| lastSelectedItem->getItemType() == FieldItemBase::player)) {
						bool groupAvailable = true;
						// Is it the same item?
						if (posibleEndItem != lastSelectedItem) {
							foreach (GroupItem *item, groupList) {
								// Check if pair already in the a group
								if ((item->getMyStartItem() == lastSelectedItem
									&& item->getMyEndItem() == posibleEndItem)
									|| (item->getMyStartItem() == posibleEndItem
									&& item->getMyEndItem() == lastSelectedItem)) {
									groupAvailable = false;      // Group already exist
								}
							}
						} else {
							groupAvailable = false;  // Same Item
						}

						if (groupAvailable == true) {
							addGroup(lastSelectedItem, posibleEndItem);
						} else {
							QMessageBox::information(NULL, "Fehler", "Gruppe schon vorhanden.");
						}
					}

					if (!(posibleEndItem->getItemType() == FieldItemBase::player
						|| lastSelectedItem->getItemType() == FieldItemBase::player)) {
						QMessageBox::information(NULL, "Logikfehler", "Mindestens ein Mitglied der Gruppe muss ein Spieler sein.");
					}
				}
				break;

			case ArrowMode:
				bool onlyArrow = true;
				foreach (ArrowItem *item, arrowList) {
					if (item->getMyStartItem() == lastSelectedItem) {  // Check if Item has a arrow
						onlyArrow = false;

						QMessageBox::information( NULL, "Logikfehler", "Dieser Figur kann nur ein Pfad zugeordnet werden.");
					}
				}
				if (onlyArrow == true) {  //
					ArrowItem *arrowItem;
					arrowItem = new ArrowItem(lastSelectedItem, mouseEvent->scenePos());
					addItem(arrowItem);
					arrowList.append(arrowItem);
					lastSelectedItem->setDestination(mouseEvent->scenePos());
				}
				break;
			}
			removeItem(tmpline);
			myLineMode = NoLineMode;
		}
	}
	updateAllItems();
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

/**
* @brief Creating the context-sensitive context menu.
*
*
*/
void FieldScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
	QGraphicsItem *itemUnderMouse = itemAt(event->scenePos(), QTransform());

	if (itemUnderMouse) {
		contextMenu.clear();
		createFieldContextMenu();
		createItemContextMenu();
		createOpponentContextMenu();
		createGroupContextMenu();

		contextMenu.addMenu(&fieldContextMenu);
		// The Item is no arrow
		if (itemUnderMouse->type() != 65546) {
			// It is a groupItem
			if (itemUnderMouse->type() == 65547) {
				contextMenu.addSeparator();
				contextMenu.addAction(QIcon(":/substitution.svg"),
				tr("Gruppe bearbeiten"), this,
				SLOT(editGroupProperties()));
			}
			// Item is a FieldItem
			if (itemUnderMouse->type() == 65550) {
				FieldItem *fieldItemUnderMouse = qgraphicsitem_cast<FieldItem *>(itemUnderMouse);

				if (fieldItemUnderMouse->getItemType() == FieldItemBase::opponent) {
					contextMenu.addSeparator();
					contextMenu.addMenu(&opponentContextMenu);
				}
				if (fieldItemUnderMouse->getItemType() == FieldItemBase::ball
					|| fieldItemUnderMouse->getItemType() == FieldItemBase::player) {
					contextMenu.addSeparator();
					contextMenu.addMenu(&itemContextMenu);
				}
			}
		}
		if (itemUnderMouse->zValue() >= 1) {
			contextMenu.addSeparator();
			contextMenu.addAction(QIcon(":/trash.svg"), tr("Löschen"), this, SLOT(removeItems()));
		}
		contextMenu.exec(event->screenPos());
	}
}

/**
* @brief Updates every item in the scene.
*
*
*/
void FieldScene::updateAllItems() {
	foreach (FieldItem *player, playerList) { player->update(); }
	foreach (FieldItem *ball, ballList) { ball->update(); }
	foreach (FieldItem *opponent, opponentList) { opponent->update(); }
	foreach (GroupItem *group, groupList) { group->updatePosition(); }
	foreach (ArrowItem *arrow, arrowList) { arrow->updatePosition(); }

	// Update Items with arrow
	foreach (FieldItem *ball, ballList) { ball->setDestination(ball->fieldPos); }
	foreach (FieldItem *player, playerList) {
		player->setDestination(player->fieldPos);
		foreach (ArrowItem *arrow, arrowList) {
			arrow->getMyStartItem()->setDestination(arrow->getMyEndPoint());
		}
	}
}

/**
* @brief Handling the items of a group.
*
*
*/
void FieldScene::editGroupProperties() {
	foreach (QGraphicsItem *item, selectedItems()) {
		if (groupList.contains(qgraphicsitem_cast<GroupItem *>(item))) {
			GroupPropertiesDialog *d = new GroupPropertiesDialog( qgraphicsitem_cast<GroupItem *>(item), fieldProperties); d->show();
		}
	}
}

/**
* @brief Creating the contectmenu if it for the field.
*
*
*/
void FieldScene::createFieldContextMenu() {
	fieldContextMenu.clear();
	fieldContextMenu.setTitle(tr("Hinzufügen"));
	fieldContextMenu.setIcon(QIcon(":/add.svg"));
	fieldContextMenu.addAction(QIcon(":/player.svg"), tr("Spieler"), this, SLOT(addPlayer()));
	fieldContextMenu.addAction(QIcon(":/opponent.svg"), tr("Gegner"), this, SLOT(addOpponent()));
	fieldContextMenu.addAction(QIcon(":/ball.svg"), tr("Ball"), this, SLOT(addBall()));
	if (playerList.empty() || opponentList.empty()) { // Show the menu only if it possible to add a team.
		createTeamContextMenu();
	}
}

/**
* @brief Creating the sub-context menu for team-options.
*
*
*/
void FieldScene::createTeamContextMenu() {
	teamContextMenu.clear();

	teamContextMenu.setTitle(tr("Team"));
	teamContextMenu.setIcon(QIcon(":/add.svg"));

	if (playerList.empty()) {
		teamContextMenu.addAction(QIcon(":/player.svg"), tr("Eigenes Team"), this, SLOT(addTeam()));
	}
	if (opponentList.empty()) {
		teamContextMenu.addAction(QIcon(":/opponent.svg"), tr("Gegnerisches Team"), this, SLOT(addOpponentTeam()));
	}
	if (playerList.empty() && opponentList.empty()) {
		teamContextMenu.addAction(QIcon(":/football58.svg"), tr("Initalisierung"), this, SLOT(addInitalisation()));
	}
	fieldContextMenu.addMenu(&teamContextMenu);
}

/**
* @brief Creating the sub-context menu for the options for a player item.
*
*
*/
void FieldScene::createItemContextMenu() {
	itemContextMenu.clear();
	itemContextMenu.setIcon(QIcon(":/redo.svg"));
	itemContextMenu.setTitle(tr("Objekt"));
	itemContextMenu.addAction(QIcon(":/arrow.svg"), tr("Pfad"), this, SLOT(activateArrowMode()));
	itemContextMenu.addAction(QIcon(":/group.svg"), tr("Gruppe"), this, SLOT(activateGroupMode()));
}

/**
* @brief Creating the sub-context menu for the options of a opponent item.
*
*
*/
void FieldScene::createOpponentContextMenu() {
	opponentContextMenu.clear();
	opponentContextMenu.setTitle(tr("Objekt"));
	opponentContextMenu.addAction(QIcon(":/group.svg"), tr("Gruppe"), this, SLOT(activateGroupMode()));
}

/**
* @brief Creating the sub-context menu for the options of a group item.
*
*
*/
void FieldScene::createGroupContextMenu() {
	groupContextMenu.clear();
	groupContextMenu.setTitle(tr("Gruppen Eigenschaften"));
	groupContextMenu.addAction(QIcon(":/substitution.svg"), tr("Bearbeiten"), this, SLOT(editGroupProperties()));
}

//
// Getter and Setter
//

QList<FieldProperties *> FieldScene::getFieldList() const {
	return myFieldList;
}

void FieldScene::setFieldList(const QList<FieldProperties *> &value) {
	myFieldList = value;
}

PreConditions *FieldScene::getPreConditions() const { return preConditions; }

void FieldScene::setPreConditions(PreConditions *value) {
	preConditions = value;
	updateFieldPropertiesForAllItems();
}

FieldProperties *FieldScene::getFieldProperties() const {
	return fieldProperties;
}

void FieldScene::setFieldProperties(FieldProperties *value) {
	fieldProperties = value;
}

QList<FieldItem *> FieldScene::getBallList() const { return ballList; }

void FieldScene::setBallList(const QList<FieldItem *> &value) {
	ballList = value;
}

QList<FieldItem *> FieldScene::getOpponentList() const { return opponentList; }

void FieldScene::setOpponentList(const QList<FieldItem *> &value) {
	opponentList = value;
}

QList<FieldItem *> FieldScene::getPlayerList() const { return playerList; }

void FieldScene::setPlayerList(const QList<FieldItem *> &value) {
	playerList = value;
}

void FieldScene::setMode(FieldScene::Mode mode) { myMode = mode; }

FieldScene::Mode FieldScene::getMode() { return myMode; }

void FieldScene::setLineMode(FieldScene::LineMode lineMode) {
	myLineMode = lineMode;
}

FieldScene::LineMode FieldScene::getLineMode() { return myLineMode; }