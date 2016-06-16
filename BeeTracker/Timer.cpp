#include "Timer.h"


Timer::Timer() {}


Timer::~Timer() {}


QTime Timer::getTime() const {
	return QTime(0, 0).addMSecs(time.elapsed());
}


void Timer::start() {
	time.start();
	timer.start(500, this);
}


void Timer::timerEvent(QTimerEvent* event) {
	emit tick(getTime());
}