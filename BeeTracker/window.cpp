#include "BeeTrackerApp.h"
#include "window.h"
#include "SplittoButton.h"
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>


Window::Window(QWidget *parent) : QMainWindow(parent) {
	auto wrapper = new QWidget(this);
	
	this->setCentralWidget(wrapper);

	auto layout = new QVBoxLayout();

	auto buttonLayout = new QHBoxLayout();
	auto previousPageButton = new QPushButton("Previous", this);
	auto nextPageButton = new QPushButton("Next", this);

	buttonLayout->addWidget(previousPageButton);
	buttonLayout->addWidget(nextPageButton);

	layout->addLayout(buttonLayout);

	layout->addWidget(&tabWidget);

	wrapper->setLayout(layout);

	tabWidget.addWidget(&designPage);
	pageOrder.append("Design");
	
	tabWidget.addWidget(&experimentPage);
	pageOrder.append("Run Experiment");
		
	tabWidget.setMinimumWidth(250);

	connect(&tabWidget, &QStackedWidget::currentChanged, [this, previousPageButton, nextPageButton](int current) {
		if (this->resizeOnTabChange) {
			for (int i = 0; i < this->tabWidget.count(); ++i) {
				if (i != current) {
					this->tabWidget.widget(i)->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
				} else {
					this->tabWidget.widget(i)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
				}
			}

			this->tabWidget.resize(this->tabWidget.minimumSizeHint());
			this->resize(this->minimumSizeHint());
			this->adjustSize();

			// Update the top buttons
			if (current > 0) {
				previousPageButton->setEnabled(true);
				previousPageButton->setText("Previous: " + this->pageOrder[current - 1]);
			} else {
				previousPageButton->setEnabled(false);
				previousPageButton->setText("");
			}
			if (current < (this->tabWidget.count() - 1)) {
				nextPageButton->setText("Next: " + this->pageOrder[current + 1]);
				nextPageButton->setEnabled(true);
			} else {
				nextPageButton->setText("");
				nextPageButton->setEnabled(false);
			}

			// TODO: Do any special config for whatever page is chosen
			if (current == 1) {
				this->experimentPage.displayDesign(*qApp->designManager.currentDesign());
			}
		}
	});

	connect(nextPageButton, &QPushButton::clicked, [this]() {
		this->tabWidget.setCurrentIndex(this->tabWidget.currentIndex() + 1);
	});
	connect(previousPageButton, &QPushButton::clicked, [this]() {
		this->tabWidget.setCurrentIndex(this->tabWidget.currentIndex() - 1);
	});
	tabWidget.currentChanged(0);

	connect(&designPage, &DesignPage::changeCurrentDesignNCols, &qApp->designManager, &DesignManager::updateCurrentDesignNCols);
	connect(&designPage, &DesignPage::changeCurrentDesignNRows, &qApp->designManager, &DesignManager::updateCurrentDesignNRows);
}


Window::~Window() {
	tabWidget.disconnect();
}