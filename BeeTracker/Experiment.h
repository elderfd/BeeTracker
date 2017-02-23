#pragma once
#include "ExperimentDesign.h"
#include "ExperimentEvent.h"
#include <vector>


class Experiment {
public:
	Experiment(const ExperimentDesign& design);
	~Experiment();

	void addEvent(const ExperimentEvent& evt);
	void addEvent(ExperimentEvent::Time time, ExperimentEvent::Type type, unsigned x, unsigned y, const QString& visitId);

private:
	const ExperimentDesign design;
	
	std::vector<ExperimentEvent> events;
};

