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

	shapeLayout->addWidget(nRowsLabel, 0, 0);
	shapeLayout->addWidget(rowSpinner, 0, 1);

	shapeLayout->addWidget(nColsLabel, 1, 0);
	shapeLayout->addWidget(colSpinner, 1, 1);

	rowSpinner->setAlignment(Qt::AlignLeft);
	colSpinner->setAlignment(Qt::AlignLeft);

	rowSpinner->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	colSpinner->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	nRowsLabel->setAlignment(Qt::AlignRight);
	nColsLabel->setAlignment(Qt::AlignRight);

	rowSpinner->setValue(ExperimentDesign::defaultNRows);
	colSpinner->setValue(ExperimentDesign::defaultNCols);

	auto addPlotTypeButton = new QPushButton("Add", this);
	auto deletePlotTypeButton = new QPushButton("Delete Selected", this);

	plotTypeModel.setDesign(qApp->designManager.currentDesign());

	auto plotTypeList = new QTableView(this);
	plotTypeList->setModel(&plotTypeModel);

	plotTypeList->setSelectionBehavior(QAbstractItemView::SelectRows);

	connect(addPlotTypeButton, &QPushButton::pressed, [this, plotTypeList]() {
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
			ExperimentDesign::PlotType newDesign;
			newDesign.name = nameInput->text();
			newDesign.displayColour = colour;

			this->plotTypeModel.beginResetModel();
			qApp->designManager.currentDesign()->plotTypes.push_back(newDesign);
			this->plotTypeModel.endResetModel();
		}
	});

	connect(deletePlotTypeButton, &QPushButton::pressed, [plotTypeList, this]() {
		// Take 0'th as can only select one at once
		auto selected = plotTypeList->selectionModel()->selectedRows()[0].row();
		auto plotType = qApp->designManager.currentDesign()->plotTypes[selected];

		auto answer = QMessageBox::question(this, "Delete", "Are you sure you wish to delete the \"" + plotType.name + "\"plot type?", QMessageBox::Yes | QMessageBox::No);

		if (answer == QMessageBox::Yes) {
			this->plotTypeModel.beginResetModel();
			qApp->designManager.currentDesign()->plotTypes.remove(selected);
			this->plotTypeModel.endResetModel();
		}
	});

	auto colourDelegate = new ColourDelegate(this);
	plotTypeList->setItemDelegateForColumn(1, colourDelegate);

	layout->addWidget(addPlotTypeButton);
	layout->addWidget(deletePlotTypeButton);
	layout->addWidget(plotTypeList);

	layout->addLayout(shapeLayout);

	layout->setAlignment(Qt::AlignTop);
}


DesignPage::~DesignPage() {}


void DesignPage::displayDesign(const ExperimentDesign& design) {
	rowSpinner->setValue(design.nRows());
	colSpinner->setValue(design.nCols());
}