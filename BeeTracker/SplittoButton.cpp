#include "SplittoButton.h"
#include "Timer.h"
#include <QHBoxLayout>
#include <QLabel>


SplittoButton::SplittoButton(QString label, const Timer& timer, QWidget* parent) : QWidget(parent), timer(timer) {
	this->setLayout(new QVBoxLayout());
	auto internalWidget = new QFrame();
	internalWidget->setFrameStyle(QFrame::Panel | QFrame::Raised);
	this->layout()->addWidget(internalWidget);

	auto layout = new QVBoxLayout(internalWidget);
	
	QFont font;
	font.setPointSize(20);
	auto numberLabel = new QLabel(label);
	numberLabel->setFont(font);
	numberLabel->setMinimumHeight(15);
	numberLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	numberLabel->setAlignment(Qt::AlignCenter);

	layout->addWidget(numberLabel);

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
	auto newButton = new SubButton(generateButtonId());
	
	newButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	newButton->setMinimumHeight(25);
	newButton->setMinimumWidth(25);

	connect(newButton, &QPushButton::pressed, [this, newButton]() {
		EventType eventType = newButton->activated ? LEAVE : ARRIVE;
		
		if (!newButton->activated) {
			newButton->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255)");
			newButton->activated = true;
			connect(&timer, &Timer::tick, newButton, &SubButton::tickLabel);
			this->addSubButton();
		} else {
			removeSubButton(newButton->id);
		}

		emit pressed(newButton->id, eventType);
	});

	buttonLayout->addWidget(newButton);
}