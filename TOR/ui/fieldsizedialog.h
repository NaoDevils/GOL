#ifndef FIELDSIZEDIALOG_H
#define FIELDSIZEDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QMouseEvent>
#include "../items/fieldproperties.h"
#include "../fieldscene.h"
#include "../items/field.h"

namespace Ui {
	class FieldSizeDialog;
}

class FieldSizeDialog : public QDialog {
	Q_OBJECT

public:
	explicit FieldSizeDialog(QList<FieldProperties *> fieldList,
	QWidget *parent = 0);
	~FieldSizeDialog();

	private slots:
	void on_pushButtonAdopt_clicked();
	void fitInView();
	void addField();
	void removeField();
	void showViewContextMenu(const QPoint &point);
	void changeField(QListWidgetItem *listWidgetItem);

    void onPushbuttongetvaluesClicked();

private:
	void fillEdits();
	void clearEdits();
	void readEdits();
	void readEdits(FieldProperties *fieldProperties);
	void fillFieldListView(QList<FieldProperties *> fieldList);
	void resizeEvent(QResizeEvent *e);

	Ui::FieldSizeDialog *ui;
	FieldProperties *myFieldProperties;
	QList<FieldProperties *> myFieldList;
	Field *myField;
	FieldScene *myFieldScene;
	QMenu *itemMenu;
};

#endif // FIELDSIZEDIALOG_H
