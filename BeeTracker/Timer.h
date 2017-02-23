#pragma once


#include "ExperimentEvent.h"
#include <QBasicTimer>
#include <QObject>
#include <QTime>


class Timer : public QObject {
	Q_OBJECT

public:
	Timer();
	~Timer();

	ExperimentEvent::Time getTime() const;

	void start();
	void pause();
	void reset();
	void stop();

signals:
	void tick(QTime newTime) const;

private:
	long long accumulatedTime = 0;
	QTime time;
	QBasicTimer timer;

	void timerEvent(QTimerEvent* event);
};

