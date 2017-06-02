#include "Experiment.h"
#include <QMap>


Experiment::Experiment() {};


Experiment::Experiment(const ExperimentDesign& design) : _design(design) {}


Experiment::~Experiment() {}


void Experiment::addEvent(const ExperimentEvent& evt) {
	events.push_back(evt);
}


void Experiment::addEvent(ExperimentEvent::Time time, ExperimentEvent::Type type, unsigned x, unsigned y, const QString& visitId) {
	events.append(ExperimentEvent(
		time,
		type,
		x,
		y,
		visitId,
		_design.xyToPlotID(x, y)
	));
}


const ExperimentDesign Experiment::design() const {
	return _design;
}


void Experiment::analyseEvents() {
	QMap<QString, Visit> visitMap;
	visits.clear();

	for (auto& evt : events) {
		auto id = evt.visitId;
		Visit* visit;

		// This should be fine as long as only one thread accessing the visitmap at once

		if (visitMap.contains(id)) {
			visit = &visitMap[id];
		} else {
			visitMap[id] = Visit();
			visit = &visitMap[id];
			visit->id = id;
		}
		
		if (evt.type == ExperimentEvent::Type::ARRIVE) {
			visit->start = evt.time;
		} else if (evt.type == ExperimentEvent::Type::LEAVE) {
			visit->end = evt.time;
		}

		// TODO: Handle undos (also need to add GUI for this)
	}

	// Sort the visits by start time
	for (auto it = visitMap.begin(); it != visitMap.end(); ++it) {
		unsigned insertAt = visits.size();

		for (unsigned i = (visits.size() - 1); i >= 0; ++i) {
			if (visits[i].start > it->start) {
				insertAt = i;
				break;
			}
		}

		it->duration = it->end - it->start;

		visits.insert(insertAt, *it);
	}
}
