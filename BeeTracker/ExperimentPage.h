#pragma once
#include "Experiment.h"
#include "OutputWriter.h"
#include "Timer.h"
#include <QWidget>


class QGridLayout;
class QLabel;
class QVBoxLayout;
class QPushButton;


class ExperimentPage : public QWidget {
	Q_OBJECT

	friend class ConfigPersistor;
public:
	ExperimentPage(QWidget* parent = nullptr);
	~ExperimentPage();

	void startExperiment();
	void pauseExperiment();
	void stopExperiment();

	void setOutputFileName(const QString& fileName);

	void displayDesign(const ExperimentDesign& design);

signals:
	void recordEvent(unsigned x, unsigned y, unsigned visitId, ExperimentEvent::Time time, ExperimentEvent::Type type);

private:
	OutputWriter output;
	Timer timer;
	QLabel* timeLabel = nullptr;
	QPushButton *goButton = nullptr, *pauseButton = nullptr, *stopButton = nullptr;
	bool paused = false;

	QGridLayout* buttonLayout = nullptr;
	QVBoxLayout* layout = nullptr;

	void resetButtons(unsigned int nRows, unsigned int nCols);
	void clearButtons();

	void setButtonsEnabled(bool enabled);
};

