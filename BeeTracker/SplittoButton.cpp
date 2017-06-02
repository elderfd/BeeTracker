#include "SplittoButton.h"
#include "Timer.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>


SplittoButton::SplittoButton(QString label, const Timer& timer, QWidget* parent) : QWidget(parent), timer(timer) {
	this->setLayout(new QVBoxLayout());
	internalWidget = new QFrame(this);
	internalWidget->setObjectName("internalWidget");
	internalWidget->setFrameStyle(QFrame::Panel | QFrame::Raised);
	this->layout()->addWidget(internalWidget);

	auto layout = new QVBoxLayout(internalWidget);
	
	QFont font;
	font.setPointSize(14);
	auto labelBackground = new QFrame(internalWidget);
	labelBackground->setObjectName("labelBackground");
	labelBackground->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	auto labelBackgroundLayout = new QVBoxLayout(labelBackground);

	auto numberLabel = new QLabel(label, labelBackground);
	numberLabel->setFont(font);
	numberLabel->setMinimumHeight(15);
	numberLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	numberLabel->setAlignment(Qt::AlignCenter);

	labelBackgroundLayout->addWidget(numberLabel);
	layout->addWidget(labelBackground);

	buttonLayout = new QHBoxLayout();
	layout->addLayout(buttonLayout);

	addSubButton();
}


SplittoButton::~SplittoButton() {}


unsigned int SplittoButton::generateButtonId() {
	// Could be issues with threading - but not expecting multiple threads to interact with button
	return nextButtonId++;
}


void SplittoButton::removeSubButton(unsigned int id) {
	for (unsigned int i = 0; i < buttonLayout->count(); i++) {
		if (((SubButton*)buttonLayout->itemAt(i)->widget())->id == id) {
			buttonLayout->takeAt(i)->widget()->deleteLater();
			break;
		}
	}
}


void SplittoButton::addSubButton() {
	auto newButton = new SubButton(generateButtonId(), this);
	
	newButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	newButton->setMinimumHeight(25);
	newButton->setMinimumWidth(25);

	connect(newButton, &QPushButton::pressed, [this, newButton]() {
		ExperimentEvent::Type eventType = newButton->activated ? ExperimentEvent::Type::LEAVE : ExperimentEvent::Type::ARRIVE;
		
		if (!newButton->activated) {
			newButton->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255)");
			newButton->activated = true;
			connect(&timer, &Timer::tick, newButton, &SubButton::tickLabel);
			this->addSubButton();
		} else {
			removeSubButton(newButton->id);
		}

		emit pressed(QString::number(newButton->id), eventType);
	});

	buttonLayout->addWidget(newButton);
}


void SplittoButton::setDisplayColour(const QColor& colour) {
	internalWidget->setStyleSheet(
		"QFrame#internalWidget{background-color: rgb(" +
		QString::number(colour.red()) +
		", " +
		QString::number(colour.green()) +
		", " +
		QString::number(colour.blue()) +
		");}QFrame{background-color: rgb(255,255,255);}"
	);
}

