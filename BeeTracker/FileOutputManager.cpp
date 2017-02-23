#include "FileOutputManager.h"
#include "pugixml.hpp"


const QString FileOutputManager::defaultEventStreamFileName = "beeEvents.csv";


FileOutputManager::FileOutputManager() {}


FileOutputManager::~FileOutputManager() {}


pugi::xml_node& operator<<(pugi::xml_node& el, const FileOutputManager& manager) {
	el.append_child("eventStreamFileName").set_value(manager.eventStreamFileName.toStdString().c_str());
	return el;
}


pugi::xml_node& operator>>(pugi::xml_node& el, FileOutputManager& manager) {
	manager.eventStreamFileName = el.child("eventStreamFileName").value();
	return el;
}


void FileOutputManager::recordExperimentEvent(const ExperimentEvent& ev) {

}