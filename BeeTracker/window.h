#pragma once


#include "AnalysisPage.h"
#include "ConfigPersistor.h"
#include "DesignPage.h"
#include "ExperimentPage.h"
#include "OutputWriter.h"
#include "SplittoButton.h"
#include "Timer.h"
#include <QStackedWidget>
#include <QMainWindow>
#include <QVector>


class Window : public QMainWindow {
	Q_OBJECT

	friend class ConfigPersistor;

public:
	Window(QWidget *parent = 0);
	~Window();

private:
	QStackedWidget tabWidget;
	DesignPage designPage;
	ExperimentPage experimentPage;
	AnalysisPage analysisPage;

	QVector<QString> pageOrder;

	bool resizeOnTabChange = false;
};
