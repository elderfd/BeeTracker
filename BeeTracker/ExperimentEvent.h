#pragma once
#include <QString>


class QTextStream;


class ExperimentEvent {
public:
	typedef long long Time;

	enum class Type {
		ARRIVE,
		LEAVE,
		UNDO
	};

	ExperimentEvent(Time time, Type type, unsigned x, unsigned y, const QString& visitId, const QString& plantId);
	~ExperimentEvent();

	friend QTextStream& operator<<(QTextStream& out, const ExperimentEvent& evt);

private:
	Time time;
	Type type;
	unsigned x, y;
	QString plantId;
	QString visitId;
};

