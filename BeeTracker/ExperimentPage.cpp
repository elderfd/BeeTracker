#include "ExperimentPage.h"
#include "SplittoButton.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "BeeTrackerApp.h"


ExperimentPage::ExperimentPage(QWidget* parent) : QWidget(parent) {
	layout = new QVBoxLayout(this);

	// Add a timer at the top
	timeLabel = new QLabel("00:00:00");
	timeLabel->setAlignment(Qt::AlignCenter);
	QFont labelFont;
	labelFont.setPointSize(20);
	timeLabel->setFont(labelFont);
	timeLabel->setMinimumWidth(200);
	timeLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	connect(&timer, &Timer::tick, [this](QTime newTime) {
		this->timeLabel->setText(newTime.toString("hh:mm:ss"));
	});

	auto fileChoiceLayout = new QHBoxLayout();
	layout->addLayout(fileChoiceLayout);
	fileChoiceLayout->setAlignment(Qt::AlignCenter);

	auto fileChoiceButton = new QPushButton("Choose output file", this);
	auto fileChoiceLabel = new QLabel("No file chosen", this);
	
	connect(fileChoiceButton, &QPushButton::pressed, [this, fileChoiceLabel]() {
		auto saveFileName = QFileDialog::getSaveFileName(
			this,
			"Choose output file"
		);

		if (!saveFileName.isEmpty()) {
			output.setOutputFileName(saveFileName);
			fileChoiceLabel->setText(saveFileName);
			stopExperiment();
			goButton->setEnabled(true);
			goButton->setToolTip("");
			pauseButton->setToolTip("");
			stopButton->setToolTip("");
		}
	});

	fileChoiceLayout->addWidget(fileChoiceButton);
	fileChoiceLayout->addWidget(fileChoiceLabel);

	QSize buttonSize(50, 50);

	goButton = new QPushButton("", this);
	goButton->setIcon(QIcon(":/images/Resources/images/playButton.svg"));
	goButton->setIconSize(buttonSize);

	pauseButton = new QPushButton("", this);
	pauseButton->setIcon(QIcon(":/images/Resources/images/pauseButton.svg"));
	pauseButton->setIconSize(buttonSize);

	stopButton = new QPushButton("", this);
	stopButton->setIcon(QIcon(":/images/Resources/images/stopButton.svg"));
	stopButton->setIconSize(buttonSize);

	goButton->setToolTip("You must specify an output file before the experiment can be started.");
	pauseButton->setToolTip("You must specify an output file before the experiment can be started.");
	stopButton->setToolTip("You must specify an output file before the experiment can be started.");

	goButton->setFixedSize(buttonSize);
	pauseButton->setFixedSize(buttonSize);
	stopButton->setFixedSize(buttonSize);

	goButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	pauseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	stopButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	connect(goButton, &QPushButton::pressed, this, &ExperimentPage::startExperiment);
	connect(pauseButton, &QPushButton::pressed, this, &ExperimentPage::pauseExperiment);
	connect(stopButton, &QPushButton::pressed, this, &ExperimentPage::stopExperiment);

	goButton->setFlat(true);
	pauseButton->setFlat(true);
	stopButton->setFlat(true);

	auto goStopLayout = new QHBoxLayout();
	goStopLayout->addWidget(timeLabel);
	goStopLayout->addWidget(goButton);
	goStopLayout->addWidget(pauseButton);
	goStopLayout->addWidget(stopButton);

	goButton->setEnabled(false);
	pauseButton->setEnabled(false);
	stopButton->setEnabled(false);

	goStopLayout->setAlignment(Qt::AlignCenter);
	goStopLayout->setSizeConstraint(QLayout::SetMinimumSize);

	layout->addLayout(goStopLayout);

	buttonLayout = new QGridLayout();
	layout->addLayout(buttonLayout);
	
	setButtonsEnabled(false);
}


ExperimentPage::~ExperimentPage() {}


void ExperimentPage::resetButtons(unsigned int nRows, unsigned int nCols) {
	clearButtons();

	for (unsigned x = 0; x < nCols; x++) {
		for (unsigned y = 0; y < nRows; y++) {
			// + 1 on label because experimentalists used to counting from 1
			QString id = qApp->designManager.currentDesign()->xyToPlotID(x, y);

			auto plotType = qApp->designManager.currentDesign()->getPlotType(y, x);

			auto button = new SplittoButton(id, timer, this);
			button->setMinimumWidth(100);
			button->setMinimumHeight(100);
			button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

			button->setDisplayColour(plotType.displayColour);

			connect(button, &SplittoButton::pressed, [this, x, y](QString visitId, ExperimentEvent::Type type) {
				ExperimentEvent evt(timer.getTime(), type, x, y, visitId, currentExperiment.design().xyToPlotID(x, y));
				currentExperiment.addEvent(evt);
				output.writeEvent(evt);
			});

			buttonLayout->addWidget(button, y, x);
		}
	}

	buttonLayout->setSizeConstraint(QLayout::SetMinimumSize);
	setButtonsEnabled(false);
}


void ExperimentPage::clearButtons() {
	if (buttonLayout != nullptr) {
		while (buttonLayout->count() > 0) {
			delete buttonLayout->takeAt(0)->widget();
		}
	}
}


void ExperimentPage::startExperiment() {
	if (paused) {
		timer.start();
		paused = false;
	} else {
		timer.reset();
		timeLabel->setText("00:00:00");
	}
	goButton->setEnabled(false);
	pauseButton->setEnabled(true);
	stopButton->setEnabled(true);

	setButtonsEnabled(true);

	output.writeHeader();
}


void ExperimentPage::pauseExperiment() {
	timer.pause();
	paused = true;
	goButton->setEnabled(true);
	pauseButton->setEnabled(false);
	stopButton->setEnabled(true);

	setButtonsEnabled(false);
}


void ExperimentPage::stopExperiment() {
	timer.stop();
	paused = false;
	goButton->setEnabled(true);
	pauseButton->setEnabled(false);
	stopButton->setEnabled(false);

	setButtonsEnabled(false);
}


void ExperimentPage::setButtonsEnabled(bool enabled) {
	for (auto i = 0; i < buttonLayout->count(); ++i) {
		buttonLayout->itemAt(i)->widget()->setEnabled(enabled);
	}
}


void ExperimentPage::setOutputFileName(const QString& fileName) {
	this->output.setOutputFileName(fileName);
}


void ExperimentPage::displayDesign(const ExperimentDesign& design) {
	currentExperiment = Experiment(design);
	resetButtons(design.nRows(), design.nCols());
}