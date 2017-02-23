#include "Timer.h"


Timer::Timer() {}


Timer::~Timer() {}


ExperimentEvent::Time Timer::getTime() const {
	return time.elapsed();
}


void Timer::start() {
	time.start();
	timer.start(500, this);
}


void Timer::timerEvent(QTimerEvent* event) {
	emit tick(QTime(0, 0).addMSecs(accumulatedTime + getTime()));
}


void Timer::reset() {
	timer.stop();
	time.restart();
	timer.start(500, this);
	accumulatedTime = 0;
}


void Timer::stop() {
	timer.stop();
}


void Timer::pause() {
	accumulatedTime += getTime();
	stop();
}