#include "ConfigPersistor.h"
#include "DesignManager.h"
#include "pugixml.hpp"


DesignManager::DesignManager() {}


DesignManager::~DesignManager() {}


DesignManager::DesignID DesignManager::getNextID() {
	nextIDMutex.lock();
	auto val = nextID++;
	nextIDMutex.unlock();
	return val;
}


DesignManager::DesignID DesignManager::addNewDesign() {
	auto newId = getNextID();
	designs[newId] = ExperimentDesign();

	if (designs.size() == 1) {
		activeID = newId;
	}

	return newId;
}


pugi::xml_node& operator<<(pugi::xml_node& el, const DesignManager& manager) {
	auto designParent = el.append_child("designs");

	for (const auto& kV : manager.designs) {
		auto id = kV.first;
		auto design = kV.second;

		auto child = designParent.append_child(std::to_string(id).c_str());
		child << design;
	}

	return el;
}


pugi::xml_node& operator>>(pugi::xml_node& el, DesignManager& manager) {
	auto designParent = el.child("designs");
	auto designNodes = designParent.children();

	for (auto& node : designNodes) {
		auto newDesign = ExperimentDesign();
		node >> newDesign;
		auto id = node.name();
		
		manager.designs[std::stoul(id)] = newDesign;
	}

	return el;
}



ExperimentDesign* DesignManager::currentDesign() {
	if (activeID == -1 || designs.size() == 0) {
		activeID = addNewDesign();
	}

	return &designs[activeID];
}


void DesignManager::updateCurrentDesignNRows(unsigned int nRows) {
	auto current = currentDesign();

	if (current != nullptr) {
		current->setNRows(nRows);
	}
}


void DesignManager::updateCurrentDesignNCols(unsigned int nCols) {
	auto current = currentDesign();

	if (current != nullptr) {
		current->setNCols(nCols);
	}
}