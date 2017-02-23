#include "ExperimentEvent.h"



ExperimentEvent::ExperimentEvent(Time time, Type type, unsigned x, unsigned y, const QString& visitId, const QString& plantId) {
	this->time = time;
	this->type = type;
	this->x = x;
	this->y = y;
	this->visitId = visitId;
	this->plantId = plantId;
}


ExperimentEvent::~ExperimentEvent() {}
