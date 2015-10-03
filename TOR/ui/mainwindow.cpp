#include <QAction>
#include <QPrinter>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fieldsizedialog.h"
#include "../fieldscene.h"
#include "../items/fieldscenelistitem.h"
#include "../items/fieldproperties.h"

/**
* @brief The mainwindow
*
*
*/
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	ui->actionSave->setEnabled(false);
	ui->actionSaveAs->setEnabled(false);
	ui->actionAdd->setEnabled(false);
	ui->actionSaveSelected->setEnabled(false);
	ui->actionExport->setEnabled(false);
	ui->changeBoardDock->hide();
	ui->propertiesDockWidget->hide();
	ui->propertiesDockWidget->setFloating(true);
	readFieldList();
	preConditions = new PreConditions;
	fieldProperties = fieldList.first();
	createDefaultsForProperties();

	scene = new FieldScene(itemMenu);

	connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
		    this,SLOT(changeSceneFromItem(QListWidgetItem *)));
	connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)),
		    this, SLOT(showViewContextMenu(QPoint)));
}

/**
* @brief Destructor
*
*
*/
MainWindow::~MainWindow() { delete ui; }

/**
* @brief Calls the write functions of every scene and saves them to a file.
*
*
*/
void MainWindow::JSONwrite(QList<FieldScene *> sceneListToExport) {
	QJsonArray jsonSceneArray;
	QJsonObject jsonObject;

	for (int index = 0; index < sceneListToExport.size(); ++index) {
		sceneListToExport[index]->JSONwrite(jsonObject);
		jsonSceneArray.append(jsonObject);
	}

	QJsonDocument saveDoc(jsonSceneArray);
	QFile saveFile(filename);
	saveFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
	saveFile.write(saveDoc.toJson());
}

/**
* @brief Load a file and creates every object in it.
*
*
*/
void MainWindow::JSONread() {
	QString filepath = QFileDialog::getOpenFileName(
	this, tr("Tafel öffnen"), QDir::homePath(), tr("Taktiktafel (*.gol)"));
	if (!filepath.isEmpty()) {
		sceneList.clear();
		FieldScene *lastScene = new FieldScene(itemMenu);
		filename = filepath;
		QFile loadFile(filepath);

		if (!loadFile.open(QIODevice::ReadOnly)) {
			qWarning("Datei konnte nicht geöffnet werden.");
		}

		QByteArray saveData = loadFile.readAll();
		QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

		QJsonArray jsonSceneArray;

		jsonSceneArray = loadDoc.array();
		for (int index = 0; index < jsonSceneArray.size(); ++index) {
			QJsonObject jsonScene = jsonSceneArray[index].toObject();
			FieldScene *newScene = new FieldScene(itemMenu, this);
			newScene->JSONread(jsonScene);

			foreach (FieldProperties *fieldPropertiesListElement, fieldList) {
				if (fieldPropertiesListElement->getName() ==
						newScene->getPreConditions()->getFieldtype()) {
					fieldProperties = fieldPropertiesListElement;
					newScene->getPreConditions()->removeImpossibleFieldProperties(fieldProperties);
				}
			}
			Field *newField = new Field(fieldProperties);
			newScene->addItem(newField);
			sceneList.append(newScene);
			addSceneListItem(newScene);
		}

		lastScene = sceneList.last();
		preConditions = lastScene->getPreConditions();
		fieldProperties = preConditions->getFieldProperties();

		ui->fieldView->setScene(lastScene);

		ui->propertiesDockWidget->show();
		ui->propertiesDockWidget->setFloating(false);
		ui->changeBoardDock->show();
		ui->listWidget->item((ui->listWidget->count() - 1))->setSelected(true);
		fitToView();
		fillFieldTypeCombobox();
		fillPreConditionComboxes();
		createDefaultsForProperties();

		connect(ui->propertiesDockWidget, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
			this, SLOT(fitToView()));
		connect(ui->changeBoardDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
			this, SLOT(fitToView()));
		connect(ui->propertiesDockWidget, SIGNAL(featuresChanged(QDockWidget::DockWidgetFeatures)),
			this, SLOT(fitToView()));
		connect(this, SIGNAL(sceneChanged(FieldScene *)),
			this, SLOT(fitToView()));
		connect(this, SIGNAL(resized()),
			this, SLOT(fitToView()));
	}
}

/**
* @brief Event for resizing the window.
*
*
*/
void MainWindow::resizeEvent(QResizeEvent *e) {
	emit resized();
	QMainWindow::resizeEvent(e);
}

/**
* @brief Adds a new item to the SceneList.
*
*
*/
void MainWindow::addSceneListItem(FieldScene *fieldScene) {
	ui->fieldView->fitInView(fieldScene->itemsBoundingRect(),
	Qt::KeepAspectRatio);
	FieldSceneListItem *listItem = new FieldSceneListItem(fieldScene);
	connect(this, SIGNAL(sceneChanged(FieldScene *)),
		listItem, SLOT(update(FieldScene *)));
	ui->listWidget->addItem(listItem);
}

/**
* @brief Reads the PreConditions of the fields.
*
*
*/
void MainWindow::savePreConditions() {
	preConditions->setName( ui->nameEdit->text() );
	preConditions->setModeToggle( ui->checkBoxMode->isChecked() );
	preConditions->setTimeToggle( ui->checkBoxGameTime->isChecked() );
	preConditions->setRemainingTimeToggle( ui->checkBoxRemainingTime->isChecked() );
	preConditions->setHalftimeToggle( ui->checkBoxHalftime->isChecked() );
	preConditions->setOwnGoalsToggle( ui->checkBoxOwnGoals->isChecked() );
	preConditions->setOppGoalsToggle( ui->checkBoxOppGoals->isChecked() );
	preConditions->setDifGoalsToggle( ui->checkBoxGoalDif->isChecked() );
	preConditions->setActivePlayerToogle( ui->checkBoxOwnPlayer->isChecked() );
	preConditions->setActiveGoalieToggle( ui->checkBoxOwnGoalie->isChecked() );
	preConditions->setActiveOpponentToogle( ui->checkBoxOppPlayer->isChecked() );
	preConditions->setActiveOppGoalieToggle( ui->checkBoxOppGoalie->isChecked() );

	preConditions->setGamemode( static_cast<PreConditions::Gamemode>( ui->comboBoxMode->currentIndex()) );
	preConditions->setHalftime( static_cast<PreConditions::Halftime>( ui->comboBoxHalftime->currentIndex()) );
	preConditions->setActiveGoalie( !!ui->comboBoxOwnGoalie->currentIndex() );
	preConditions->setActiveOpponentGoalie( !!ui->comboBoxOppGoalie->currentIndex() );

	preConditions->setGameTimeMin( ui->spinBoxGameTimeMin->value() );
	preConditions->setGameTimeMax( ui->spinBoxGameTimeMax->value() );
	preConditions->setGameTimeRemainingMin( ui->spinBoxRemainingTimeMin->value() );
	preConditions->setGameTimeRemainingMax( ui->spinBoxRemainingTimeMax->value() );
	preConditions->setOwnGoalsMin( ui->spinBoxOwnGoalsMin->value() );
	preConditions->setOwnGoalsMax( ui->spinBoxOwnGoalsMax->value() );
	preConditions->setOppGoalsMin( ui->spinBoxOppGoalsMin->value() );
	preConditions->setOppGoalsMax( ui->spinBoxOppGoalsMax->value() );
	preConditions->setDifGoalsMin( ui->spinBoxGoalDifMin->value() );
	preConditions->setDifGoalsMax( ui->spinBoxGoalDifMax->value() );
	preConditions->setActivePlayerMin( ui->spinBoxOwnPlayerMin->value() );
	preConditions->setActivePlayerMax( ui->spinBoxOwnPlayerMax->value() );
	preConditions->setActiveOpponentMin( ui->spinBoxOppPlayerMin->value() );
	preConditions->setActiveOpponentMax( ui->spinBoxOppPlayerMax->value() );

	preConditions->setNote( ui->textEditNote->toPlainText() );

	preConditions->setFieldtype( ui->comboBoxFieldtyp->currentText() );

	emit updatePreConditons(preConditions);
	ui->propertiesDockWidget->setFloating(false);
}

/**
* @brief Fills the ComboBox for the fieldtyps.
*
*
*/
void MainWindow::fillFieldTypeCombobox() {
	ui->comboBoxFieldtyp->clear();
	if (fieldList.isEmpty()) {
		fieldList.append(preConditions->getFieldProperties());
	}

	for (int i = 0; i < fieldList.size(); ++i) {
		ui->comboBoxFieldtyp->addItem(fieldList.at(i)->getName());
	}
}

/**
* @brief Loads the saved PreConditions into the corresponding objects from the GUI.
*
*
*/
void MainWindow::fillPreConditionComboxes() {
	ui->comboBoxMode->clear();
	QMetaObject metaGamemode = preConditions->staticMetaObject;
	QMetaEnum metaEnumGamemode =
	metaGamemode.enumerator(metaGamemode.indexOfEnumerator("Gamemode"));

	for (int i = 0; i < metaEnumGamemode.keyCount(); ++i) {
		ui->comboBoxMode->addItem(metaEnumGamemode.key(i));
	};

	ui->comboBoxHalftime->clear();
	QMetaObject metaHalftime = preConditions->staticMetaObject;
	QMetaEnum metaEnumHalftime =
	metaHalftime.enumerator(metaHalftime.indexOfEnumerator("Halftime"));
	for (int i = 0; i < metaEnumHalftime.keyCount(); ++i) {
		ui->comboBoxHalftime->addItem(metaEnumHalftime.key(i));
	}

	ui->comboBoxOwnGoalie->clear();
	ui->comboBoxOwnGoalie->addItem("false");
	ui->comboBoxOwnGoalie->addItem("true");

	ui->comboBoxOppGoalie->clear();
	ui->comboBoxOppGoalie->addItem("false");
	ui->comboBoxOppGoalie->addItem("true");
}

/**
* @brief Updating of the scene. Emits a Signal.
*
*
*/
void MainWindow::updateScene() {
	fieldProperties = preConditions->getFieldProperties();
	scene->setPreConditions(preConditions);
	emit sceneChanged(scene);
}

/**
* @brief Fits the field in the visible area.
*
*
*/
void MainWindow::fitToView() {
	ui->fieldView->fitInView(ui->fieldView->scene()->itemsBoundingRect(),
	Qt::KeepAspectRatio);
}

/**
* @brief Select every scene from the SceneList.
*
*
*/
void MainWindow::selectAllScenes() {
	for (int index = 0; index < ui->listWidget->count(); ++index) {
		ui->listWidget->item(index)->setCheckState(Qt::Checked);
	}
}

/**
* @brief Reads the field-typs from the file fieldlist.rule.
*
*
*/
void MainWindow::readFieldList() {
	QDir dir(QCoreApplication::applicationDirPath());
	if (!dir.cd("config")) {
		qWarning("Ordner kann nicht geöffnet werden.");
	} else {
		QFile file(dir.filePath("fieldlist.rule"));
		if (!file.open(QIODevice::ReadOnly)) {
			qWarning("Datei kann nicht gelesen werden");
		} else {
			QByteArray saveData = file.readAll();

			QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

			QJsonArray jsonArrayFields = loadDoc.array();

			for (int index = 0; index < jsonArrayFields.size(); ++index) {
				QJsonObject jsonField = jsonArrayFields[index].toObject();
				FieldProperties *tempFieldProperties = new FieldProperties;
				tempFieldProperties->read(jsonField);
				fieldList.append(tempFieldProperties);
			}
		}
	}
	if (fieldList.empty()) {
		fieldList.append(fieldProperties);
	}
}

/**
* @brief Getter for the FieldList.
*
*
*/

QList<FieldProperties *> MainWindow::getFieldList() const { return fieldList; }

/**
* @brief Setter for the FieldList.
*
*
*/
void MainWindow::setFieldList(const QList<FieldProperties *> &value) {
	fieldList = value;
}

/**
* @brief Open a file.
*
*
*/
void MainWindow::on_actionOpen_triggered() {
	ui->actionSave->setEnabled(true);
	ui->actionSaveAs->setEnabled(true);
	ui->actionAdd->setEnabled(true);
	ui->actionSaveSelected->setEnabled(true);
	ui->actionExport->setEnabled(true);
	ui->listWidget->clear();
	JSONread();
}

/**
* @brief Menu-Entry: Save.
*
*
*/
void MainWindow::on_actionSave_triggered() { JSONwrite(sceneList); }

/**
* @brief Exporting the fields to a PDF-file.
*
*
*/
void MainWindow::on_actionExport_triggered() {
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOrientation(QPrinter::Landscape);
	printer.setPageSize(QPrinter::A4);
	QString filepath = QFileDialog::getExistingDirectory( this, tr("Tafeln exportieren"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!filepath.isEmpty()) {
		for (int index = 0; index < ui->listWidget->count(); ++index) {
			if (ui->listWidget->item(index)->checkState() == Qt::Checked) {
				FieldScene *printScene = dynamic_cast<FieldSceneListItem *>(ui->listWidget->item(index)) ->getMyFieldScene();
				QString filename = printScene->getPreConditions()->getName();
				printer.setOutputFileName(filepath + "\\" + filename + ".pdf");
				QPainter p;
				p.begin(&printer);
				printScene->render(&p);
				p.end();
			}
		}
	}
}

/**
* @brief Menu-Entry: New
*
*
*/
void MainWindow::on_actionNew_triggered() {
	ui->actionSaveAs->setEnabled(true);
	ui->actionAdd->setEnabled(true);
	ui->actionSaveSelected->setEnabled(true);
	ui->actionExport->setEnabled(true);
	ui->listWidget->clear();
	sceneList.clear();
	on_actionAdd_triggered();
}

/**
* @brief Adding action.
*
*
*/
void MainWindow::on_actionAdd_triggered() {
	ui->actionSaveAs->setEnabled(true);
	preConditions = new PreConditions();
	createPreCondtions();
	ui->changeBoardDock->show();
}

/**
* @brief Initial dialog for PreConditions.
*
*
*/
void MainWindow::createPreCondtions() {
	ui->propertiesDockWidget->show();
	ui->propertiesDockWidget->setFloating(true);
	ui->propertiesDockWidget->move(	QApplication::desktop()->screen()->rect().center() - ui->propertiesDockWidget->rect().center());
	connect(this, SIGNAL(updatePreConditons(PreConditions *)),
			this,  SLOT(createScene()));
	fillFieldTypeCombobox();
	fillPreConditionComboxes();
}

/**
* @brief Creating a scene.
*
*
*/
void MainWindow::createScene() {
	fieldProperties = preConditions->getFieldProperties();
	Field *newField = new Field(fieldProperties);
	FieldScene *newScene = new FieldScene(itemMenu, this);
	sceneList.append(newScene);
	newScene->setPreConditions(preConditions);
	newScene->setFieldList(fieldList);
	newScene->addItem(newField);
	ui->fieldView->setScene(newScene);
	ui->changeBoardDock->show();
	addSceneListItem(newScene);

	disconnect(this, SIGNAL(updatePreConditons(PreConditions *)),
			   this, SLOT(createScene()));
	connect(this, SIGNAL(updatePreConditons(PreConditions *)),
			this, SLOT(updateScene()));
	connect(ui->propertiesDockWidget, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
			this, SLOT(fitToView()));
	connect(ui->changeBoardDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
			this, SLOT(fitToView()));
	connect(ui->propertiesDockWidget, SIGNAL(featuresChanged(QDockWidget::DockWidgetFeatures)),
			this, SLOT(fitToView()));
	connect(this, SIGNAL(sceneChanged(FieldScene *)),
			this, SLOT(fitToView()));
	connect(this, SIGNAL(resized()),
			this, SLOT(fitToView()));
}

/**
* @brief The action for Save as.
*
*
*/
void MainWindow::on_actionSaveAs_triggered() {
	ui->actionSave->setEnabled(true);
	QString filepath = QFileDialog::getSaveFileName(
	this, tr("Tafel speichern"), QDir::homePath(), tr("Taktikmappe (*.gol)"));

	if (!filepath.isEmpty()) {
		filename = filepath;
		on_actionSave_triggered();
	}
}

/**
* @brief Loads the Scene from a SceneListItem.
*
*
*/
void MainWindow::changeSceneFromItem(QListWidgetItem *item) {
	FieldSceneListItem *newItem = dynamic_cast<FieldSceneListItem *>(item);
	FieldScene *tempScene = newItem->getMyFieldScene();
	preConditions = tempScene->getPreConditions();
	fieldProperties = preConditions->getFieldProperties();
	createDefaultsForProperties();
	ui->fieldView->setScene(tempScene);
	fitToView();
}

/**
* @brief Action for SavenSelected.
*
*
*/
void MainWindow::on_actionSaveSelected_triggered() {
	bool fieldCheck = true;

	QString filepath = QFileDialog::getSaveFileName( this, tr("Tafel speichern"), QDir::homePath(), tr("Taktikmappe (*.gol)"));

	if (!filepath.isEmpty()) {
		filename = filepath;
		QList<FieldScene *> selectedScenes;

		for (int index = 0; index < ui->listWidget->count(); ++index) {
			if (ui->listWidget->item(index)->checkState() == Qt::Checked) {
				selectedScenes.append( dynamic_cast<FieldSceneListItem *>(ui->listWidget->item(index)) ->getMyFieldScene());
				if (selectedScenes.first()->getPreConditions()->getFieldtype() != selectedScenes.last()->getPreConditions()->getFieldtype()) {
					fieldCheck = false;
				}
			}
		}
		JSONwrite(selectedScenes);

		if (fieldCheck == false) {
			QMessageBox::information(NULL, "Vorsicht",
			"Mappe enthält verschiedene Feldtypen.");
		}
	}
}

/**
* @brief The ContextMenu for the ListView.
*
*
*/
void MainWindow::showViewContextMenu(const QPoint &point) {
	QMenu menu;
	menu.addAction(QIcon(":/add.svg"), tr("Hinzufügen"), this, SLOT(on_actionAdd_triggered()));

	if (ui->listWidget->itemAt(point)) { menu.addAction(QIcon(":/trash.svg"), tr("Löschen"), this, SLOT(removeScene())); }
	menu.addAction(tr("Alle auswählen"), this, SLOT(selectAllScenes()));
	menu.exec(ui->listWidget->viewport()->mapToGlobal(point));
}

/**
* @brief Remove scene from SceneList.
*
*
*/
void MainWindow::removeScene() {
	foreach (QListWidgetItem *item, ui->listWidget->selectedItems()) {
		FieldScene *scene = (dynamic_cast<FieldSceneListItem *>(item))->getMyFieldScene();
		sceneList.removeAll(scene);
		ui->listWidget->takeItem(ui->listWidget->row(item));
	}
}

/**
* @brief Resets the shown scene.
*
*
*/
void MainWindow::resetScene() { ui->fieldView->setScene(scene); }

/**
* @brief Creates the default-values for FieldProperties.
*
*
*/
void MainWindow::createDefaultsForProperties() {
	ui->comboBoxFieldtyp->setCurrentText(preConditions->getFieldProperties()->getName() );
	ui->spinBoxGameTimeMin->setMaximum( preConditions->getFieldProperties()->getHalftimeDuration() );
	ui->spinBoxGameTimeMax->setMaximum( preConditions->getFieldProperties()->getHalftimeDuration() );
	ui->spinBoxRemainingTimeMin->setMaximum( preConditions->getFieldProperties()->getHalftimeDuration() );
	ui->spinBoxRemainingTimeMax->setMaximum( preConditions->getFieldProperties()->getHalftimeDuration() );
	ui->spinBoxOwnPlayerMin->setMaximum( preConditions->getFieldProperties()->getTeamSize() );
	ui->spinBoxOwnPlayerMax->setMaximum( preConditions->getFieldProperties()->getTeamSize() );
	ui->spinBoxOppPlayerMin->setMaximum( preConditions->getFieldProperties()->getTeamSize() );
	ui->spinBoxOppPlayerMax->setMaximum( preConditions->getFieldProperties()->getTeamSize() );

	ui->checkBoxMode->setChecked( preConditions->getModeToggle() );
	ui->checkBoxGameTime->setChecked( preConditions->getTimeToggle() );
	ui->checkBoxRemainingTime->setChecked( preConditions->getRemainingTimeToggle() );
	ui->checkBoxHalftime->setChecked( preConditions->getHalftimeToggle() );
	ui->checkBoxOwnGoals->setChecked( preConditions->getOwnGoalsToggle() );
	ui->checkBoxOppGoals->setChecked( preConditions->getOppGoalsToggle() );
	ui->checkBoxGoalDif->setChecked( preConditions->getDifGoalsToggle() );
	ui->checkBoxOwnPlayer->setChecked( preConditions->getActivePlayerToogle() );
	ui->checkBoxOwnGoalie->setChecked( preConditions->getActiveGoalieToggle() );
	ui->checkBoxOppPlayer->setChecked( preConditions->getActiveOpponentToogle() );
	ui->checkBoxOppGoalie->setChecked( preConditions->getActiveOppGoalieToggle() );

	ui->comboBoxMode->setCurrentIndex( preConditions->getGamemode() );
	ui->comboBoxHalftime->setCurrentIndex( preConditions->getHalftime() );
	ui->comboBoxOppGoalie->setCurrentIndex( preConditions->getActiveGoalie() );
	ui->comboBoxOwnGoalie->setCurrentIndex( preConditions->getActiveOpponentGoalie() );

	ui->spinBoxGameTimeMin->setValue( preConditions->getGameTimeMin() );
	ui->spinBoxGameTimeMax->setValue( preConditions->getGameTimeMax() );
	ui->spinBoxRemainingTimeMin->setValue( preConditions->getGameTimeRemainingMin() );
	ui->spinBoxRemainingTimeMax->setValue( preConditions->getGameTimeRemainingMax() );
	ui->spinBoxOwnGoalsMin->setValue( preConditions->getOwnGoalsMin() );
	ui->spinBoxOwnGoalsMax->setValue( preConditions->getOwnGoalsMax() );
	ui->spinBoxOppGoalsMin->setValue( preConditions->getOppGoalsMin() );
	ui->spinBoxOppGoalsMax->setValue( preConditions->getOppGoalsMax() );
	ui->spinBoxGoalDifMin->setValue( preConditions->getDifGoalsMin() );
	ui->spinBoxGoalDifMax->setValue( preConditions->getDifGoalsMax() );
	ui->spinBoxOwnPlayerMin->setValue( preConditions->getActivePlayerMin() );
	ui->spinBoxOwnPlayerMax->setValue( preConditions->getActivePlayerMax() );
	ui->spinBoxOppPlayerMin->setValue( preConditions->getActiveOpponentMin() );
	ui->spinBoxOppPlayerMax->setValue( preConditions->getActiveOpponentMax() );

	ui->textEditNote->setText( preConditions->getNote() );
	ui->nameEdit->setText( preConditions->getName() );
}

/**
* @brief Opens the dialog for changing the Fields.
*
*
*/
void MainWindow::on_actionEditFields_triggered() {
	FieldSizeDialog *d = new FieldSizeDialog(fieldList);
	d->show();
}

/**
* @brief Save the PreConditions.
*
*
*/
void MainWindow::on_pushButtonAdoptPreConditions_clicked() {
	savePreConditions();
}

/**
* @brief Creating the combo box with the names of the Fields.
*
*
*/
void MainWindow::on_comboBoxFieldtyp_currentTextChanged(const QString &arg1) {
	foreach (FieldProperties *item, fieldList) {
		if (arg1 == item->getName()) {
			preConditions->removeImpossibleFieldProperties(item);
			preConditions->setFieldtype(preConditions->getFieldProperties()->getName());
			createDefaultsForProperties();
		}
	}
}