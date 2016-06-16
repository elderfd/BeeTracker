#pragma once


#include <QBasicTimer>
#include <QObject>
#include <QTime>


class Timer : public QObject {
	Q_OBJECT

public:
	Timer();
	~Timer();

	QTime getTime() const;

	void start();
	void stop();

signals:
	void tick(QTime newTime) const;

private:
	QTime time;
	QBasicTimer timer;

	void timerEvent(QTimerEvent* event);
};

