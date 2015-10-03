#include "fieldsizedialog.h"
#include "ui_fieldsizedialog.h"
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include "../items/fieldlistitem.h"

/**
* @brief Dialog for editing the Fields.
*
*
*/
FieldSizeDialog::FieldSizeDialog(QList<FieldProperties *> fieldList, QWidget *parent) : QDialog(parent), ui(new Ui::FieldSizeDialog) {
  myFieldList = fieldList;
  myFieldProperties = fieldList.first();
  myField = new Field(myFieldProperties);
  myFieldScene = new FieldScene(itemMenu, this);
  myFieldScene->addItem(myField);
  setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
  ui->setupUi(this);
  ui->fieldView->setScene(myFieldScene);
  myField->hide();
  fillEdits();
  fillFieldListView(myFieldList);
  update();
  connect(ui->fieldListWidget, SIGNAL(itemSelectionChanged()),
		  this, SLOT(fitInView()));
  connect(ui->fieldListWidget, SIGNAL(customContextMenuRequested(QPoint)),
		  this, SLOT(showViewContextMenu(QPoint)));
  connect(ui->fieldListWidget, SIGNAL(itemClicked(QListWidgetItem *)),
		  this, SLOT(changeField(QListWidgetItem *)));
}

/**
* @brief Destructor
*
*
*/
FieldSizeDialog::~FieldSizeDialog() { delete ui; }

/**
* @brief Fill the edit-fields with the values of the active field.
*
*
*/
void FieldSizeDialog::fillEdits() {
  ui->editFieldName->setText( myFieldProperties->getName() );
  ui->spinBoxBallSize->setValue( myFieldProperties->getBallSize() );
  ui->spinBoxBorderStripWidth->setValue( myFieldProperties->getBorderStripWidth() );
  ui->spinBoxCenterCircleDiameter->setValue( myFieldProperties->getCenterCircleDiameter() );
  ui->spinBoxFieldLength->setValue( myFieldProperties->getFieldLength() );
  ui->spinBoxFieldWidth->setValue( myFieldProperties->getFieldWidth() );
  ui->spinBoxGoalPostSize->setValue( myFieldProperties->getGoalPostSize() );
  ui->spinBoxGoalRoom->setValue( myFieldProperties->getGoalRoom() );
  ui->spinBoxGoalWidth->setValue( myFieldProperties->getGoalWidth() );
  ui->spinBoxHalftimeDuration->setValue( myFieldProperties->getHalftimeDuration() );
  ui->spinBoxLineWidth->setValue( myFieldProperties->getLineWidth() );
  ui->spinBoxPenaltyAreaLength->setValue( myFieldProperties->getPenaltyAreaLength() );
  ui->spinBoxPenaltyAreaWidth->setValue( myFieldProperties->getPenaltyAreaWidth() );
  ui->spinBoxPenaltyMarkDistance->setValue( myFieldProperties->getPenaltyMarkDistance() );
  ui->spinBoxPenaltyMarkSize->setValue( myFieldProperties->getPenaltyMarkSize() );
  ui->spinBoxPlayerSize->setValue( myFieldProperties->getPlayerSize() );
  ui->spinBoxTeamSize->setValue( myFieldProperties->getTeamSize() );
}

/**
* @brief Clears the field-edits.
*
*
*/
void FieldSizeDialog::clearEdits() {
  ui->editFieldName->setText("<Neu>");
  ui->spinBoxBallSize->clear();
  ui->spinBoxBorderStripWidth->clear();
  ui->spinBoxCenterCircleDiameter->clear();
  ui->spinBoxFieldLength->clear();
  ui->spinBoxFieldWidth->clear();
  ui->spinBoxGoalPostSize->clear();
  ui->spinBoxGoalRoom->clear();
  ui->spinBoxGoalWidth->clear();
  ui->spinBoxHalftimeDuration->clear();
  ui->spinBoxLineWidth->clear();
  ui->spinBoxPenaltyAreaLength->clear();
  ui->spinBoxPenaltyAreaWidth->clear();
  ui->spinBoxPenaltyMarkDistance->clear();
  ui->spinBoxPenaltyMarkSize->clear();
  ui->spinBoxPlayerSize->clear();
  ui->spinBoxTeamSize->clear();
}

/**
* @brief Read edits.
*
*
*/
void FieldSizeDialog::readEdits() {
	readEdits(myFieldProperties);
}

/**
* @brief Read the edits and save them in a FieldProperties object.
*
*
*/
void FieldSizeDialog::readEdits(FieldProperties *fieldProperties) {
  fieldProperties->setName( ui->editFieldName->text() );
  fieldProperties->setBallSize( ui->spinBoxBallSize->value() );
  fieldProperties->setBorderStripWidth( ui->spinBoxBorderStripWidth->value() );
  fieldProperties->setCenterCircleDiameter( ui->spinBoxCenterCircleDiameter->value() );

  fieldProperties->setFieldLength( ui->spinBoxFieldLength->value() );
  fieldProperties->setFieldWidth( ui->spinBoxFieldWidth->value() );
  fieldProperties->setGoalPostSize( ui->spinBoxGoalPostSize->value() );
  fieldProperties->setGoalRoom( ui->spinBoxGoalRoom->value() );
  fieldProperties->setGoalWidth( ui->spinBoxGoalWidth->value() );
  fieldProperties->setHalftimeDuration( ui->spinBoxHalftimeDuration->value() );
  fieldProperties->setLineWidth( ui->spinBoxLineWidth->value() );
  fieldProperties->setPenaltyAreaLength( ui->spinBoxPenaltyAreaLength->value() );
  fieldProperties->setPenaltyAreaWidth( ui->spinBoxPenaltyAreaWidth->value() );
  fieldProperties->setPenaltyMarkDistance( ui->spinBoxPenaltyMarkDistance->value() );
  fieldProperties->setPenaltyMarkSize( ui->spinBoxPenaltyMarkSize->value() );
  fieldProperties->setPlayerSize( ui->spinBoxPlayerSize->value() );
  fieldProperties->setTeamSize( ui->spinBoxTeamSize->value() );
}

/**
* @brief Lists all available fields.
*
*
*/
void FieldSizeDialog::fillFieldListView(QList<FieldProperties *> fieldList) {
  ui->fieldListWidget->clear();
  if (fieldList.isEmpty()) {
	fieldList.append(myFieldProperties);
  }
  for (int i = 0; i < fieldList.count(); ++i) {
	FieldListItem *tempFieldListItem = new FieldListItem(fieldList[i]);
	ui->fieldListWidget->addItem(tempFieldListItem);
  }
}

/**
* @brief Event for resizing the window.
*
*
*/
void FieldSizeDialog::resizeEvent(QResizeEvent *e) {
  ui->fieldView->fitInView(myFieldScene->sceneRect(), Qt::KeepAspectRatio);
  QDialog::resizeEvent(e);
}

/**
* @brief The context menu for adding the and deleting fields.
*
*
*/
void FieldSizeDialog::showViewContextMenu(const QPoint &point) {
  QMenu menu;
  menu.addAction(QIcon(":add.svg"), tr("Hinzufügen"), this, SLOT(addField()));
  if (ui->fieldListWidget->itemAt(point)) {
	menu.addAction(QIcon(":/trash.svg"), tr("Löschen"), this, SLOT(removeField()));
  }
  menu.exec(ui->fieldListWidget->viewport()->mapToGlobal(point));
}

/**
* @brief Change the shown field.
*
*
*/
void FieldSizeDialog::changeField(QListWidgetItem *listWidgetItem) {
  myFieldProperties = dynamic_cast<FieldListItem *>(listWidgetItem)->getMyFieldProperties();
  fillEdits();
  Field *newField = new Field(myFieldProperties);
  myFieldScene = new FieldScene(itemMenu);
  ui->fieldView->setScene(myFieldScene);
  myFieldScene->addItem(newField);
  fitInView();
}

/**
* @brief Save the fields to a file.
*
*
*/
void FieldSizeDialog::on_pushButtonAdopt_clicked() {
  QDir dir(QCoreApplication::applicationDirPath());
  if (!dir.cd("config")) {
	qWarning("Kann den Ordner nicht finden.");
  } else {
	QFile saveFile(dir.filePath("fieldlist.rule"));
	QJsonArray jsonFieldArray;
	QJsonObject jsonObject;

	for (int index = 0; index < myFieldList.size(); ++index) {
	  myFieldList[index]->write(jsonObject);
	  jsonFieldArray.append(jsonObject);
	}

	QJsonDocument saveDoc(jsonFieldArray);
	saveFile.open(QIODevice::WriteOnly);
	saveFile.write(saveDoc.toJson());
  }
  close();
}

/**
* @brief Fit the view in the visible area.
*
*
*/
void FieldSizeDialog::fitInView() {
  myField->show();
  ui->fieldView->fitInView(myFieldScene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

/**
* @brief Creates a new default field.
*
*
*/
void FieldSizeDialog::addField() {
  FieldProperties *tempFieldProperties = new FieldProperties;
  clearEdits();
  readEdits(tempFieldProperties);
  myFieldList.append(tempFieldProperties);
  fillFieldListView(myFieldList);
}

/**
* @brief Delete field from list.
*
*
*/
void FieldSizeDialog::removeField() {
  clearEdits();
  FieldProperties *tempFieldProperties = dynamic_cast<FieldListItem *>( ui->fieldListWidget->selectedItems().first())->getMyFieldProperties();
  readEdits(tempFieldProperties);
  myFieldList.removeAll(tempFieldProperties);
  fillFieldListView(myFieldList);
}

/**
* @brief Save the values.
*
*
*/
void FieldSizeDialog::onPushbuttongetvaluesClicked() {
  FieldProperties *tempFieldProperties = dynamic_cast<FieldListItem *>( ui->fieldListWidget->selectedItems().first())->getMyFieldProperties();
  readEdits(tempFieldProperties);
  dynamic_cast<FieldListItem *>(ui->fieldListWidget->selectedItems().first()) ->setMyFieldProperties( tempFieldProperties);
  ui->fieldListWidget->selectedItems().first()->setText( tempFieldProperties->getName());
}