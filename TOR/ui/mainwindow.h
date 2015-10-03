#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSvg>
#include <QJsonObject>
#include "../fieldscene.h"
#include "../items/field.h"
#include "../items/fieldproperties.h"
#include "../items/fielditem.h"
#include "../items/fieldscenelistitem.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void JSONwrite(QList<FieldScene *> sceneListToExport);
	void JSONread();

	QList<FieldProperties *> getFieldList() const;
	void setFieldList(const QList<FieldProperties *> &value);

signals:
	void updatePreConditons(PreConditions* preConditions);
	void sceneChanged(FieldScene* scene);
	void resized();

	private slots:

	void on_actionNew_triggered();
	void on_actionAdd_triggered();
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionSaveAs_triggered();
	void on_actionSaveSelected_triggered();
	void on_actionExport_triggered();
	void on_actionEditFields_triggered();

	void changeSceneFromItem(QListWidgetItem *item);
	void showViewContextMenu(const QPoint &point);
	void createScene();
	void updateScene();
	void removeScene();
	void resetScene();
	void fitToView();
	void selectAllScenes();

	void on_pushButtonAdoptPreConditions_clicked();

	void on_comboBoxFieldtyp_currentTextChanged(const QString &arg1);

private:
	void resizeEvent(QResizeEvent *e);

	void readFieldList();
	void addSceneListItem(FieldScene *fieldScene);
	void createSceneListMenu(QMenu *menu);

	void createPreCondtions();
	void savePreConditions();
	void fillFieldTypeCombobox();
	void fillPreConditionComboxes();

	void createDefaultsForProperties();

	Ui::MainWindow *ui;
	QString filename;
	FieldScene *scene;
	QList<FieldScene *> sceneList;
	Field *field;
	QList<FieldProperties *> fieldList;
	FieldProperties *fieldProperties;
	PreConditions *preConditions;
	QJsonArray jsonTacticBoard;
	QJsonArray jsonPlayerArray;
	QList<FieldItem *> playerList;
	QJsonArray jsonOpponentArray;
	QList<FieldItem *> opponentList;
	QJsonArray jsonBallArray;
	QList<FieldItem *> ballList;
	QMenu *itemMenu;
	QMenu *delAction;
};

#endif // MAINWINDOW_H
