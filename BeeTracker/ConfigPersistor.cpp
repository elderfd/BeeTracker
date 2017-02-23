#include "BeeTrackerApp.h"
#include "ConfigPersistor.h"
#include "pugixml.hpp"


const QString ConfigPersistor::fileName = ".beesbeesbees.xml";


ConfigPersistor::ConfigPersistor() {
	pugi::xml_document doc;
	auto result = doc.load_file(fileName.toStdString().c_str());
	
	if (result.status) {
		doc.child("designManager") >> qApp->designManager;
		doc.child("outputManager") >> qApp->fileOutputManager;
	} else {
		// Default settings
		qApp->designManager.addNewDesign();
	}
}


ConfigPersistor::~ConfigPersistor() {
	pugi::xml_document doc;
	doc.append_child("designManager") << qApp->designManager;
	doc.append_child("outputManager") << qApp->fileOutputManager;
	doc.save_file(fileName.toStdString().c_str());
}