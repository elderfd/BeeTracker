#pragma once
#include "ExperimentDesign.h"
#include <map>
#include <QMutex>
#include <QObject>


namespace pugi {
	class xml_node;
}


class DesignManager : public QObject {
	Q_OBJECT

public:
	DesignManager();
	~DesignManager();

	typedef unsigned DesignID;

	DesignID addNewDesign();

	void updateCurrentDesignNRows(unsigned int nRows);
	void updateCurrentDesignNCols(unsigned int nCols);

	friend pugi::xml_node& operator<<(pugi::xml_node& el, const DesignManager& manager);
	friend pugi::xml_node& operator>>(pugi::xml_node& el, DesignManager& manager);
	
	ExperimentDesign* currentDesign();

private:
	QMutex nextIDMutex;
	DesignID nextID = 0;
	DesignID getNextID();

	std::map<unsigned, ExperimentDesign> designs;
	DesignID activeID = -1;
};

