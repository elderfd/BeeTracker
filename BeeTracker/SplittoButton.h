#pragma once


#include <QWidget>
#include <QPushButton>


class Timer;


class SplittoButton : public QWidget {
	Q_OBJECT

public:
	SplittoButton(QString label, const Timer& timer, QWidget *parent = 0);
	~SplittoButton();

	void removeSubButton(unsigned int id);
	void subButtonPressed();

	enum EventType {
		ARRIVE,
		LEAVE
	};

signals:
	void pressed(unsigned int buttonID, EventType type);

private:
	const Timer& timer;
	QLayout* buttonLayout;
	unsigned int nextButtonId = 1;

	void addSubButton();
	unsigned int generateButtonId();
};


class SubButton : public QPushButton {
	Q_OBJECT

public:
	SubButton(unsigned int id, QWidget *parent = 0) : QPushButton(parent) {
		this->id = id;
	}

	void tickLabel() {
		this->setText(QString::number(double(++labelTime) / 2, 'f', 1));
	}

	int id;
	bool activated = false;

protected:
	unsigned labelTime = 0;
};

