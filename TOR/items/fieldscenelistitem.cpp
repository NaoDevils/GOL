#include "fieldscenelistitem.h"
#include <qDebug>

/**
* @brief The Items for the SceneList
*
*/
FieldSceneListItem::FieldSceneListItem(FieldScene *fieldScene) {
	myFieldScene = fieldScene;
	createIcon();
	setText(myFieldScene->getPreConditions()->getName());
	setFlags(flags() | Qt::ItemIsUserCheckable);
	setCheckState(Qt::Unchecked);
	QObject::connect(myFieldScene, SIGNAL(selectionChanged()), this, SLOT(update())); // If something in the own FieldScene has changed update the object.
}

/**
* @brief Update all items.
*
*/
void FieldSceneListItem::update()
{
	createIcon();
	refreshScene();
	setText(myFieldScene->getPreConditions()->getName());
}

/**
* @brief Draw a preview item for the saved scene.
*
*/
void FieldSceneListItem::createIcon() {
	int pixmapWidth = 300;
	QPixmap *image = new QPixmap(pixmapWidth, pixmapWidth * (myFieldScene->height() / myFieldScene->width() ));
	QPainter painter(image);
	painter.setRenderHint(QPainter::Antialiasing);
	myFieldScene->render(&painter);
	setIcon(*image);
}

/**
* @brief Load new properties to the Items of the scene.
*
*/
void FieldSceneListItem::refreshScene()
{
	myFieldScene->updateFieldPropertiesForAllItems();
}



