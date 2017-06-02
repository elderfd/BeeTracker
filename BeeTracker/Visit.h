#pragma once
#include "ExperimentEvent.h"


class Visit {
public:
	Visit();
	~Visit();

	friend QTextStream& operator<<(QTextStream& out, const Visit& visit);

	ExperimentEvent::Time start;
	ExperimentEvent::Time end;
	ExperimentEvent::Time duration;
	unsigned x, y;
	QString id;
};

