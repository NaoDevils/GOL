#include "grouppropertiesdialog.h"
#include "ui_grouppropertiesdialog.h"
#include "../items/groupitem.h"
#include <QDebug>

/**
* @brief The Dialog for the GroupProperties.
*
*
*/
GroupPropertiesDialog::GroupPropertiesDialog(GroupItem *groupItem, FieldProperties *fieldProperties, QWidget *parent) : QDialog(parent), ui(new Ui::GroupPropertiesDialog) {
	setWindowFlags(Qt::WindowTitleHint);
	myFieldProperties = fieldProperties;
	myGroupItem = groupItem;
	ui->setupUi(this);
	createDefaults();
}

/**
* @brief Destructor.
*
*
*/
GroupPropertiesDialog::~GroupPropertiesDialog() { delete ui; }

/**
* @brief Changes the leader from the combo-box.
*
*
*/
void GroupPropertiesDialog::on_leaderComboBox_activated(const QString &arg1) {
	if (arg1 == myGroupItem->startItem()->getName()) {
	} else {
		FieldItem *tmpItem = new FieldItem(FieldItemBase::misc, myGroupItem->startItem()->getMyFieldProperties());
		tmpItem = myGroupItem->getMyStartItem();
		myGroupItem->setMyStartItem(myGroupItem->getMyEndItem());
		myGroupItem->setMyEndItem(tmpItem);
		ui->followerLabel->setText(myGroupItem->getMyEndItem()->getName());
	}
}

/**
* @brief Saves the changes.
*
*
*/
void GroupPropertiesDialog::on_acceptButton_clicked() {
	myGroupItem->setXDistance(ui->xSpinBox->value());
	myGroupItem->setYDistance(ui->ySpinBox->value());
	close();
}

/**
* @brief Creates the initial values for the dialog.
*
*
*/
void GroupPropertiesDialog::createDefaults() {
	if (!myGroupItem->getMyEndItem()->getItemType() == FieldItemBase::player) {
		FieldItem *tmpItem = new FieldItem(FieldItemBase::misc, myGroupItem->startItem()->getMyFieldProperties());
		tmpItem = myGroupItem->getMyStartItem();
		myGroupItem->setMyStartItem(myGroupItem->getMyEndItem());
		myGroupItem->setMyEndItem(tmpItem);
		ui->leaderComboBox->addItem(myGroupItem->getMyStartItem()->getName());
	} else if (myGroupItem->getMyStartItem()->getItemType() == FieldItemBase::player) {
		ui->leaderComboBox->addItem(myGroupItem->getMyStartItem()->getName());
		ui->leaderComboBox->addItem(myGroupItem->getMyEndItem()->getName());
		} else {ui->leaderComboBox->addItem(myGroupItem->getMyStartItem()->getName());}
	ui->followerLabel->setText(myGroupItem->getMyEndItem()->getName());
	ui->xSpinBox->setValue(myGroupItem->getXDistance());
	ui->ySpinBox->setValue(myGroupItem->getYDistance());
	ui->ySpinBox->setMinimum(-myFieldProperties->getFieldWidth());
	ui->ySpinBox->setMaximum(myFieldProperties->getFieldWidth());
	ui->xSpinBox->setMinimum(-myFieldProperties->getFieldLength());
	ui->xSpinBox->setMaximum(myFieldProperties->getFieldLength());
}