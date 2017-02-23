#include "Experiment.h"


Experiment::Experiment(const ExperimentDesign& design) : design(design) {}


Experiment::~Experiment() {}


void Experiment::addEvent(const ExperimentEvent& evt) {
	events.push_back(evt);
}


void Experiment::addEvent(ExperimentEvent::Time time, ExperimentEvent::Type type, unsigned x, unsigned y, const QString& visitId) {
	events.emplace_back(
		time,
		type,
		x,
		y,
		visitId,
		design.xyToPlotID(x, y)
	);
}