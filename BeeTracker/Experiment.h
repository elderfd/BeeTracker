#pragma once
#include "ExperimentDesign.h"
#include "ExperimentEvent.h"
#include <QVector>
#include "Visit.h"


class Experiment {
	friend class OutputWriter;

public:
	Experiment();
	Experiment(const ExperimentDesign& design);
	~Experiment();

	void addEvent(const ExperimentEvent& evt);
	void addEvent(ExperimentEvent::Time time, ExperimentEvent::Type type, unsigned x, unsigned y, const QString& visitId);

	const ExperimentDesign design() const;

	void analyseEvents();

private:
	ExperimentDesign _design;
	
	QVector<ExperimentEvent> events;
	QVector<Visit> visits;
};

