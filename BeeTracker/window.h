#pragma once


#include "OutputWriter.h"
#include <QtWidgets/QMainWindow>
#include "SplittoButton.h"
#include "Timer.h"


class QLayoutItem;


class Window : public QMainWindow {
	Q_OBJECT

public:
	Window(QWidget *parent = 0);
	~Window();

private:
	OutputWriter writer;
	QWidget centralWidget;
	Timer timer;
	const unsigned int numberOfRows = 10,
		numberOfCols = 5;

	void buttonPressed(unsigned int plantId, unsigned int visitId, QTime time, SplittoButton::EventType type);
	void clearCentralWidgetLayout();
	void constructExperimentUI();
	void constructStartUI();
	static void emptyLayoutItem(QLayoutItem* item);
};
