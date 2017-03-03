#include "ColourDelegate.h"
#include "DesignPage.h"
#include "Experiment.h"
#include "ExperimentDesign.h"
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include "BeeTrackerApp.h"
#include <QMessageBox>
#include <QScrollArea>
#include <QHeaderView>
#include <QGroupBox>
#include <QSplitter>


DesignPage::DesignPage(QWidget* parent) : QWidget(parent) {
	layout = new QVBoxLayout(this);

	auto shapeLayout = new QGridLayout();

	auto nRowsLabel = new QLabel("Number of rows:", this);
	auto nColsLabel = new QLabel("Number of columns:", this);

	rowSpinner = new QSpinBox(this);
	colSpinner = new QSpinBox(this);

	rowSpinner->setMinimum(1);
	colSpinner->setMinimum(1);

	rowSpinner->setMaximum(ExperimentDesign::maxSize);
	colSpinner->setMaximum(ExperimentDesign::maxSize);

	connect(rowSpinner, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DesignPage::changeCurrentDesignNRows);
	connect(colSpinner, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DesignPage::changeCurrentDesignNCols);

	shapeLayout->addWidget(nRowsLabel, 0, 0, Qt::AlignTop | Qt::AlignRight);
	shapeLayout->addWidget(rowSpinner, 0, 1, Qt::AlignTop | Qt::AlignLeft);

	shapeLayout->addWidget(nColsLabel, 1, 0, Qt::AlignTop | Qt::AlignRight);
	shapeLayout->addWidget(colSpinner, 1, 1, Qt::AlignTop | Qt::AlignLeft);

	rowSpinner->setAlignment(Qt::AlignLeft);
	colSpinner->setAlignment(Qt::AlignLeft);

	rowSpinner->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
	colSpinner->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

	nRowsLabel->setAlignment(Qt::AlignRight);
	nColsLabel->setAlignment(Qt::AlignRight);

	rowSpinner->setValue(ExperimentDesign::defaultNRows);
	colSpinner->setValue(ExperimentDesign::defaultNCols);

	rowSpinner->setMaximumWidth(200);
	colSpinner->setMaximumWidth(200);
	rowSpinner->setMinimumWidth(50);
	colSpinner->setMinimumWidth(50);
	nRowsLabel->setMaximumWidth(200);
	nColsLabel->setMaximumWidth(200);

	auto plotLayoutBox = new QGroupBox("Plot Layout", this);
	layout->addWidget(plotLayoutBox);

	auto addPlotTypeButton = new QPushButton("Add", this);
	auto deletePlotTypeButton = new QPushButton("Delete Selected", this);
	auto editPlotTypeButton = new QPushButton("Edit Selected", this);

	addPlotTypeButton->setMaximumWidth(150);
	deletePlotTypeButton->setMaximumWidth(150);
	editPlotTypeButton->setMaximumWidth(150);
	
	addPlotTypeButton->setMinimumWidth(100);
	deletePlotTypeButton->setMinimumWidth(100);
	editPlotTypeButton->setMinimumWidth(100);

	plotTypeModel.setDesign(qApp->designManager.currentDesign());

	plotTypeList = new QTableView(this);
	plotTypeList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	plotTypeList->setModel(&plotTypeModel);

	plotTypeList->setSelectionBehavior(QAbstractItemView::SelectRows);

	connect(addPlotTypeButton, &QPushButton::pressed, [this]() {
		auto dialog = new QDialog(this);
		dialog->setModal(true);

		auto layout = new QGridLayout(dialog);

		layout->addWidget(new QLabel("Name: ", dialog), 0, 0);
		layout->addWidget(new QLabel("Color: ", dialog), 1, 0);

		auto nameInput = new QLineEdit(dialog);
		layout->addWidget(nameInput, 0, 1);

		auto colourInput = new QPushButton("", dialog);
		layout->addWidget(colourInput, 1, 1);
		
		colourInput->setFlat(true);
		colourInput->setAutoFillBackground(true);
		QColor colour = Qt::white;
		auto pal = colourInput->palette();
		pal.setColor(QPalette::Button, colour);
		colourInput->setPalette(pal);

		connect(colourInput, &QPushButton::pressed, [dialog, colourInput, &colour]() {
			auto chosenColour = QColorDialog::getColor(Qt::white, dialog);

			if (chosenColour.isValid()) {
				colour = chosenColour;
				auto pal = colourInput->palette();
				pal.setColor(QPalette::Button, colour);
				colourInput->setPalette(pal);
			}
		});

		auto acceptAddButton = new QPushButton("Add", dialog);
		auto cancelAddButton = new QPushButton("Cancel", dialog);

		layout->addWidget(acceptAddButton, 2, 0);
		layout->addWidget(cancelAddButton, 2, 1);
		
		bool addConfirmed = false;

		connect(acceptAddButton, &QPushButton::pressed, [&addConfirmed, dialog]() {
			addConfirmed = true;
			dialog->close();
		});

		connect(cancelAddButton, &QPushButton::pressed, [dialog]() {
			dialog->close();
		});

		dialog->exec();

		if (addConfirmed) {
			// TODO: Need to check that plot type with this name doesn't already exist - validator on input would be good
			ExperimentDesign::PlotType newDesign;
			newDesign.name = nameInput->text();
			newDesign.displayColour = colour;

			this->plotTypeModel.beginResetModel();
			qApp->designManager.currentDesign()->addPlotType(newDesign);
			this->plotTypeModel.endResetModel();
		}
	});

	connect(deletePlotTypeButton, &QPushButton::pressed, [this]() {
		// Take 0'th as can only select one at once
		if (qApp->designManager.currentDesign()->getAllPlotTypes().size() == 1) {
			QMessageBox msg;
			msg.setText("You cannot delete the last plot type.");
			msg.exec();
			return;
		}

		auto selected = plotTypeList->selectionModel()->selectedRows()[0].row();
		auto plotType = qApp->designManager.currentDesign()->getAllPlotTypes()[selected];

		auto answer = QMessageBox::question(this, "Delete", "Are you sure you wish to delete the \"" + plotType.name + "\"plot type?", QMessageBox::Yes | QMessageBox::No);

		if (answer == QMessageBox::Yes) {
			this->plotTypeModel.beginResetModel();
			qApp->designManager.currentDesign()->deletePlotType(selected);
			this->plotTypeModel.endResetModel();
		}
	});

	connect(editPlotTypeButton, &QPushButton::pressed, [this]() {
		auto selected = plotTypeList->selectionModel()->selectedRows()[0].row();
		auto plotType = qApp->designManager.currentDesign()->getAllPlotTypes()[selected];

		auto dialog = new QDialog(this);
		dialog->setModal(true);

		auto layout = new QGridLayout(dialog);

		layout->addWidget(new QLabel("Name: ", dialog), 0, 0);
		layout->addWidget(new QLabel("Color: ", dialog), 1, 0);

		auto nameInput = new QLineEdit(plotType.name, dialog);
		layout->addWidget(nameInput, 0, 1);

		auto colourInput = new QPushButton("", dialog);
		layout->addWidget(colourInput, 1, 1);

		colourInput->setFlat(true);
		colourInput->setAutoFillBackground(true);
		QColor colour = plotType.displayColour;
		auto pal = colourInput->palette();
		pal.setColor(QPalette::Button, colour);
		colourInput->setPalette(pal);

		connect(colourInput, &QPushButton::pressed, [dialog, colourInput, &colour, plotType]() {
			auto chosenColour = QColorDialog::getColor(plotType.displayColour, dialog);

			if (chosenColour.isValid()) {
				colour = chosenColour;
				auto pal = colourInput->palette();
				pal.setColor(QPalette::Button, colour);
				colourInput->setPalette(pal);
			}
		});

		auto acceptAddButton = new QPushButton("Accept", dialog);
		auto cancelAddButton = new QPushButton("Cancel", dialog);

		layout->addWidget(acceptAddButton, 2, 0);
		layout->addWidget(cancelAddButton, 2, 1);

		bool editConfirmed = false;

		connect(acceptAddButton, &QPushButton::pressed, [&editConfirmed, dialog]() {
			editConfirmed = true;
			dialog->close();
		});

		connect(cancelAddButton, &QPushButton::pressed, [dialog]() {
			dialog->close();
		});

		dialog->exec();

		if (editConfirmed) {
			ExperimentDesign::PlotType newDesign;
			newDesign.name = nameInput->text();
			newDesign.displayColour = colour;

			this->plotTypeModel.beginResetModel();
			qApp->designManager.currentDesign()->editPlotType(selected, newDesign);
			this->plotTypeModel.endResetModel();

			this->displayDesign(*qApp->designManager.currentDesign());
		}
	});

	auto colourDelegate = new ColourDelegate(this);
	plotTypeList->setItemDelegateForColumn(1, colourDelegate);

	plotTypeList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

	plotLayoutBox->setLayout(shapeLayout);

	auto plotTypeBox = new QGroupBox("Plot Types", this);
	auto plotTypeBoxLayout = new QVBoxLayout(plotTypeBox);

	auto plotTypeListResizer = new QSplitter(this);
	plotTypeListResizer->setOrientation(Qt::Vertical);

	plotTypeBoxLayout->addWidget(addPlotTypeButton);
	plotTypeBoxLayout->addWidget(deletePlotTypeButton);
	plotTypeBoxLayout->addWidget(editPlotTypeButton);
	plotTypeBoxLayout->addWidget(plotTypeListResizer);

	plotTypeBoxLayout->setAlignment(addPlotTypeButton, Qt::AlignCenter);
	plotTypeBoxLayout->setAlignment(deletePlotTypeButton, Qt::AlignCenter);
	plotTypeBoxLayout->setAlignment(editPlotTypeButton, Qt::AlignCenter);

	plotTypeListResizer->addWidget(plotTypeList);

	layout->addWidget(plotTypeBox);

	// Grid to allow setting of plot type
	auto plotGridScroll = new QScrollArea(this);
	auto plotGridContent = new QWidget(this);
	
	plotGridContent->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	plotGridLayout = new QGridLayout(plotGridContent);

	plotTypeListResizer->addWidget(plotGridScroll);

	plotGridScroll->setLayout(new QHBoxLayout());
	plotGridScroll->setWidget(plotGridContent);
	plotGridContent->layout()->setAlignment(Qt::AlignCenter);
	plotGridScroll->layout()->setAlignment(Qt::AlignCenter);
	plotGridScroll->setWidgetResizable(true);

	connect(rowSpinner, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this]() {
		this->displayDesign(*qApp->designManager.currentDesign());
	});
	connect(colSpinner, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this]() {
		this->displayDesign(*qApp->designManager.currentDesign());
	});

	this->displayDesign(*qApp->designManager.currentDesign());
}


DesignPage::~DesignPage() {}


void DesignPage::displayDesign(const ExperimentDesign& design) {
	rowSpinner->setValue(design.nRows());
	colSpinner->setValue(design.nCols());

	if (plotGridLayout != nullptr) {
		while (plotGridLayout->count() > 0) {
			delete plotGridLayout->takeAt(0)->widget();
		}
	}

	for (unsigned int i = 0; i < design.nRows(); ++i) {
		for (unsigned int j = 0; j < design.nCols(); j++) {
			QString id = qApp->designManager.currentDesign()->xyToPlotID(i, j);
			
			auto buttonWrapper = new QWidget(this);
			auto buttonWrapperLayout = new QVBoxLayout(buttonWrapper);

			buttonWrapperLayout->addWidget(new QLabel(id, buttonWrapper));

			auto button = new QPushButton("", buttonWrapper);
			button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

			buttonWrapperLayout->addWidget(button);

			auto plotType = qApp->designManager.currentDesign()->getPlotType(i, j);

			button->setStyleSheet(
				"background-color: rgb(" +
				QString::number(plotType.displayColour.red()) +
				", " +
				QString::number(plotType.displayColour.green()) +
				", " +
				QString::number(plotType.displayColour.blue()) +
				");"
			);

			connect(button, &QPushButton::pressed, [id, this, i, j]() {
				if (plotTypeList->selectionModel()->selectedRows().size() > 0) {
					auto selected = plotTypeList->selectionModel()->selectedRows()[0].row();
					auto plotType = qApp->designManager.currentDesign()->getAllPlotTypes()[selected];

					qApp->designManager.currentDesign()->setPlotType(i, j, plotType);
					this->displayDesign(*qApp->designManager.currentDesign());
				}
			});

			plotGridLayout->addWidget(buttonWrapper, i, j);
		}
	}

	plotGridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
}