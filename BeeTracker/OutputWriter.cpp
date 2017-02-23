#include "ExperimentEvent.h"
#include "OutputWriter.h"
#include <QFile>
#include <QTextStream>


OutputWriter::OutputWriter() {}


OutputWriter::~OutputWriter() {}


const QString& OutputWriter::getOutputFileName() const {
	return outputFileName;
}


void OutputWriter::setOutputFileName(const QString& newName) {
	outputFileName = newName;
}


void OutputWriter::writeHeader() {
	QFile outFile(outputFileName);

	if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		throw std::runtime_error("Failed to open output file!");
	}

	QTextStream outStream(&outFile);

	outStream << "Plant ID" << sep << "Plant X" << sep << "Plant Y" << sep << "Visit ID" << sep << "Time/msec" << sep << "Arrive/Leave\n";

	outFile.close();
}


void OutputWriter::writeEvent(const ExperimentEvent& evt) {
	if(!QFile::exists(outputFileName)) writeHeader();
	
	QFile outFile(outputFileName);

	if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
		throw std::runtime_error("Failed to open output file!");
	}

	QTextStream outStream(&outFile);

	outStream << evt << "\n";

	outFile.close();
}


QTextStream& operator<<(QTextStream& out, const ExperimentEvent& evt) {
	out << evt.plantId << OutputWriter::sep
		<< evt.x << OutputWriter::sep
		<< evt.y << OutputWriter::sep
		<< evt.visitId << OutputWriter::sep
		<< evt.time << OutputWriter::sep;
	
	if (evt.type == ExperimentEvent::Type::ARRIVE) {
		out << "A";
	} else if(evt.type == ExperimentEvent::Type::LEAVE) {
		out << "L";
	} else {
		out << "U";
	}

	return out;
}