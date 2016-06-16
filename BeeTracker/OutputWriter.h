#pragma once

#include <QObject>
#include <QString>
#include <QTime>


class OutputWriter : public QObject {
	Q_OBJECT

public:
	OutputWriter();
	~OutputWriter();

	const QString& getOutputFileName() const;
	void setOutputFileName(QString newName);

	void writeHeader();
	void writeArriveEvent(QString plantId, QString visitId, QTime time);
	void writeLeaveEvent(QString plantId, QString visitId, QTime time);

private:
	QString outputFileName;
	const char sep = '\t';
};

