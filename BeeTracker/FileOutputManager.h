#pragma once
#include <QString>
#include "OutputWriter.h"


namespace pugi {
	class xml_node;
}


class ExperimentEvent;


class FileOutputManager {
public:
	FileOutputManager();
	~FileOutputManager();

	friend pugi::xml_node& operator<<(pugi::xml_node& el, const FileOutputManager& manager);
	friend pugi::xml_node& operator>>(pugi::xml_node& el, FileOutputManager& manager);

	void recordExperimentEvent(const ExperimentEvent& ev);

private:
	static const QString defaultEventStreamFileName;

	QString eventStreamFileName = defaultEventStreamFileName;

	OutputWriter writer;
};

