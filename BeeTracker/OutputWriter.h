#pragma once

#include <QObject>
#include <QString>
#include <QTime>


class ExperimentEvent;


class OutputWriter : public QObject {
	Q_OBJECT

public:
	OutputWriter();
	~OutputWriter();

	const QString& getOutputFileName() const;
	void setOutputFileName(const QString& newName);

	void writeHeader();
	void writeEvent(const ExperimentEvent& evt);

	static const char sep = '\t';
private:
	QString outputFileName;
};

