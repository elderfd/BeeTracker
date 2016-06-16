#include "OutputWriter.h"
#include <QFile>
#include <QTextStream>


OutputWriter::OutputWriter() {}


OutputWriter::~OutputWriter() {}


const QString& OutputWriter::getOutputFileName() const {
	return outputFileName;
}


void OutputWriter::setOutputFileName(QString newName) {
	outputFileName = newName;
}


void OutputWriter::writeHeader() {
	QFile outFile(outputFileName);

	if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		throw std::runtime_error("Failed to open output file!");
	}

	QTextStream outStream(&outFile);

	outStream << "Plant ID" << sep << "Visit ID" << sep << "Time" << sep << "Time/msec" << sep << "Arrive/Leave\n";

	outFile.close();
}


void OutputWriter::writeArriveEvent(QString plantId, QString visitId, QTime time) {
	QFile outFile(outputFileName);

	if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
		throw std::runtime_error("Failed to open output file!");
	}

	QTextStream outStream(&outFile);

	outStream << plantId << sep << visitId << sep << time.toString("hh:mm:ss:zzz") << sep << time.msecsSinceStartOfDay() << sep << "A\n";

	outFile.close();
}


void OutputWriter::writeLeaveEvent(QString plantId, QString visitId, QTime time) {
	QFile outFile(outputFileName);

	if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
		throw std::runtime_error("Failed to open output file!");
	}

	QTextStream outStream(&outFile);

	outStream << plantId << sep << visitId << sep << time.toString("hh:mm:ss:zzz") << sep << time.msecsSinceStartOfDay() << sep << "L\n";

	outFile.close();
}