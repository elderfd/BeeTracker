#include "ExperimentDesign.h"
#include "pugixml.hpp"


const ExperimentDesign::PlotType ExperimentDesign::defaultPlotType = {
	QString("Default"),
	QColor(256, 256, 256)
};


ExperimentDesign::ExperimentDesign() {
	// Default initialise
	_nRows = defaultNRows;
	_nCols = defaultNCols;
	plotTypes = { defaultPlotType };
}


ExperimentDesign::~ExperimentDesign() {}


void ExperimentDesign::setNRows(unsigned rows) {
	_nRows = rows;
}


void ExperimentDesign::setNCols(unsigned cols) {
	_nCols = cols;
}


unsigned ExperimentDesign::nRows() const {
	return _nRows;
}


unsigned ExperimentDesign::nCols() const {
	return _nCols;
}



pugi::xml_node& operator<<(pugi::xml_node& el, const ExperimentDesign& design) {
	el.append_child("nRows").set_value(std::to_string(design._nRows).c_str());
	el.append_child("nCols").set_value(std::to_string(design._nCols).c_str());
	return el;
}


pugi::xml_node& operator>>(pugi::xml_node& el, ExperimentDesign& design) {
	design._nRows = std::stoul(el.child("nRows").value());
	design._nCols = std::stoul(el.child("nCols").value());

	return el;
}


QString ExperimentDesign::xyToPlotID(unsigned int x, unsigned int y) const {
	return QString::number(y * _nRows + x);
}