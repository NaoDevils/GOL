#ifndef NUMBERDIALOG_H
#define NUMBERDIALOG_H

#include <QDialog>
#include "../items/fielditem.h"
#include "../items/fieldproperties.h"

namespace Ui {
	class NumberDialog;
}

class NumberDialog : public QDialog {
	Q_OBJECT

public:
	explicit NumberDialog(FieldItem *item, QList<FieldItem *> itemList,
	FieldProperties *fieldProperties, QWidget *parent = 0);
	~NumberDialog();

	private slots:
	void on_pushButton_clicked();

	void on_comboBox_currentTextChanged(const QString &arg1);

private:
	void createPossibleNumbers();

	Ui::NumberDialog *ui;
	FieldItem *myItem;
	QList<FieldItem *> myItemList;
	FieldProperties *myFieldProperties;
};

#endif // NUMBERDIALOG_H
