#pragma once
#include <QString>


class QTextStream;


class ExperimentEvent {
	friend class Experiment;

public:
	typedef long long Time;

	enum class Type {
		ARRIVE,
		LEAVE,
		UNDO
	};

	ExperimentEvent();
	ExperimentEvent(Time time, Type type, unsigned x, unsigned y, const QString& visitId, const QString& plantId);
	~ExperimentEvent();

	friend QTextStream& operator<<(QTextStream& out, const ExperimentEvent& evt);

private:
	Time time = 0;
	Type type = Type::ARRIVE;
	unsigned x = 0, y = 0;
	QString plantId;
	QString visitId;
};

