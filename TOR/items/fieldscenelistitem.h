#ifndef FIELDSCENELISTITEM_H
#define FIELDSCENELISTITEM_H

#include <QListWidgetItem>
#include "../fieldscene.h"

class FieldSceneListItem : public QObject,  public QListWidgetItem
{
	Q_OBJECT
public:
	FieldSceneListItem(FieldScene *fieldScene);

	// Getter & Setter
    FieldScene *getMyFieldScene() const { return myFieldScene; }
    void setMyFieldScene(FieldScene *value) { myFieldScene = value; }

public slots:
	void update();
	void refreshScene();

private:

	void createIcon();
	FieldScene *myFieldScene;
};

#endif // FIELDSCENELISTITEM_H
