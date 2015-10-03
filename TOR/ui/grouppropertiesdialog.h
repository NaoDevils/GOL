#ifndef GROUPPROPERTIESDIALOG_H
#define GROUPPROPERTIESDIALOG_H

#include <QDialog>
#include "../items/groupitem.h"
#include "../items/fieldproperties.h"

namespace Ui {
	class GroupPropertiesDialog;
}

class GroupPropertiesDialog : public QDialog {
	Q_OBJECT

public:
	explicit GroupPropertiesDialog(GroupItem *groupItem,
	FieldProperties *fieldProperties,
	QWidget *parent = 0);
	~GroupPropertiesDialog();

	private slots:
	void on_leaderComboBox_activated(const QString &arg1);
	void on_acceptButton_clicked();

private:
	void createDefaults();
	Ui::GroupPropertiesDialog *ui;
	GroupItem *myGroupItem;
	FieldProperties *myFieldProperties;
	int groupDistanceX;
	int groupDistanceY;
};

#endif // GROUPPROPERTIESDIALOG_H
