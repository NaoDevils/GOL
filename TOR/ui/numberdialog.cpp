#include "numberdialog.h"
#include "ui_numberdialog.h"
#include <QDebug>

/**
* @brief Dialog for selecting the number of a player and an opponent.
*
*
*/

NumberDialog::NumberDialog(FieldItem *item, QList<FieldItem *> itemList, FieldProperties *fieldProperties, QWidget *parent) : QDialog(parent), ui(new Ui::NumberDialog) {
	setWindowFlags(Qt::WindowTitleHint);
	myItem = item;
	myFieldProperties = fieldProperties;
	myItemList = itemList;
	ui->setupUi(this);
	createPossibleNumbers();
}

/**
* @brief Destructor
*
*
*/
NumberDialog::~NumberDialog() { delete ui; }

/**
* @brief Save and close if button pushed.
*
*
*/
void NumberDialog::on_pushButton_clicked() {
	myItem->setName(ui->comboBox->currentText());
	close();
}

/**
* @brief Creates the numbers which can be selected.
*
*
*/
void NumberDialog::createPossibleNumbers() {
	QStringList itemNumbers;

	if (static_cast<FieldItemBase::ItemType>(myItem->getItemType()) == FieldItemBase::player) {
		for (int i = 1; i <= myFieldProperties->getTeamSize(); i++) {
			itemNumbers.append(QString("P%1").arg(i));
		}

		foreach (FieldItem *player, myItemList) {
			itemNumbers.removeAll(player->getName());
		}
	} else {
		for (int i = 1; i <= myFieldProperties->getTeamSize(); i++) {
			itemNumbers.append(QString("O%1").arg(i));
		}

		foreach (FieldItem *opponent, myItemList) {
			itemNumbers.removeAll(opponent->getName());
		}
	}
	ui->comboBox->addItems(itemNumbers);
}

/**
* @brief Shows informations of the selected number.
*
*
*/
void NumberDialog::on_comboBox_currentTextChanged(const QString &arg1)
{
	if (arg1 == "O1" || arg1 == "P1" ){
		ui->labelPlayertyp->setText("Vorgemerkt für Torwart");
	} else{
		ui->labelPlayertyp->setText("Feldspieler");
	}
}