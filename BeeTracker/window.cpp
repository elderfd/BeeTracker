#include "window.h"
#include "SplittoButton.h"
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>


Window::Window(QWidget *parent)
	: QMainWindow(parent) {
	this->setCentralWidget(&centralWidget);

	constructStartUI();
}


Window::~Window() {

}


void Window::constructStartUI() {
	clearCentralWidgetLayout();

	auto mainLayout = new QVBoxLayout(&centralWidget);

	// Must prompt user for output file name
	QHBoxLayout* fileChoiceLayout = new QHBoxLayout();
	mainLayout->addLayout(fileChoiceLayout);
	fileChoiceLayout->setAlignment(Qt::AlignCenter);
	
	QPushButton* fileChoiceButton = new QPushButton("Choose output file");
	QLabel* fileChoiceLabel = new QLabel("No file chosen");
	QPushButton* goButton = new QPushButton("Start experiment");

	connect(fileChoiceButton, &QPushButton::pressed, [this, fileChoiceLabel]() {
		auto saveFileName = QFileDialog::getSaveFileName(
			this,
			"Choose output file"
		);

		if (!saveFileName.isEmpty()) {
			writer.setOutputFileName(saveFileName);
			fileChoiceLabel->setText(saveFileName);
		}
	});

	connect(goButton, &QPushButton::pressed, [this]() {
		if (writer.getOutputFileName().isEmpty()) {
			QMessageBox msg;
			msg.setText("No output file has been set!");
			msg.exec();
		} else {
			constructExperimentUI();
		}
	});

	fileChoiceLayout->addWidget(fileChoiceButton);
	fileChoiceLayout->addWidget(fileChoiceLabel);
	mainLayout->addWidget(goButton);
	fileChoiceLayout->setAlignment(Qt::AlignCenter);
	mainLayout->setAlignment(Qt::AlignCenter);
}


void Window::constructExperimentUI() {
	clearCentralWidgetLayout();

	// Add a timer at the top
	auto timeLabel = new QLabel("00:00:00");
	timeLabel->setAlignment(Qt::AlignCenter);
	QFont labelFont;
	labelFont.setPointSize(20);
	timeLabel->setFont(labelFont);

	connect(&timer, &Timer::tick, [timeLabel](QTime newTime) {
		timeLabel->setText(newTime.toString("hh:mm:ss"));
	});

	centralWidget.layout()->addWidget(timeLabel);

	// Add the display of the experimental grid
	auto gridLayout = new QGridLayout();

	for (unsigned int x = 0; x < numberOfCols; x++) {
		for (unsigned int y = 0; y < numberOfRows; y++) {
			// + 1 on label because experimentalists used to counting from 1
			unsigned int id = x * numberOfRows + y + 1;
			
			auto button = new SplittoButton(QString::number(id), timer);
			button->setMinimumWidth(75);
			button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
			
			connect(button, &SplittoButton::pressed, [this, id](unsigned int visitId, SplittoButton::EventType type) {
				buttonPressed(id, visitId, this->timer.getTime(), type);
			});
			
			// TODO: Do some colouring to help readability
			// if (x % 2 != y % 2) button->setStyleSheet("background-color: rgb(150, 150, 150);");

			gridLayout->addWidget(button, y, x);
		}
	}

	((QVBoxLayout*)centralWidget.layout())->addLayout(gridLayout);

	// Start the experiment going
	writer.writeHeader();
	timer.start();
}


void Window::clearCentralWidgetLayout() {
	if (centralWidget.layout() != nullptr) {
		while (centralWidget.layout()->count() > 0) {
			emptyLayoutItem(centralWidget.layout()->takeAt(0));
		}
	}
}


void Window::buttonPressed(unsigned int plantId, unsigned int visitId, QTime time, SplittoButton::EventType type) {
	if (type == SplittoButton::ARRIVE) {
		writer.writeArriveEvent(QString::number(plantId), QString::number(visitId), time);
	} else {
		writer.writeLeaveEvent(QString::number(plantId), QString::number(visitId), time);
	}
}


void Window::emptyLayoutItem(QLayoutItem* item) {
	if (QLayout* layout = item->layout()) {
		while (layout->count() > 0) {
			emptyLayoutItem(layout->takeAt(0));
		}
	} else {
		item->widget()->deleteLater();
	}
}