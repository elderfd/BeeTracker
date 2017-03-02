#include "ExperimentDesign.h"
#include "pugixml.hpp"
#include <QException>


const ExperimentDesign::PlotType ExperimentDesign::defaultPlotType = {
	QString("Default"),
	QColor(255, 255, 255)
};


ExperimentDesign::ExperimentDesign() {
	// Default initialise
	plotTypes = { defaultPlotType };
	setSize(defaultNRows, defaultNCols);
}


ExperimentDesign::~ExperimentDesign() {}


void ExperimentDesign::setNRows(unsigned rows) {
	_nRows = rows;

	while(rows >= assignedPlotTypes.size()) {
		QVector<PlotType> newRow;
		
		for (unsigned i = 0; i < _nCols; ++i) {
			newRow.append(plotTypes[0]);
		}

		assignedPlotTypes.append(newRow);
	}

	while (assignedPlotTypes.size() > rows) {
		assignedPlotTypes.pop_back();
	}
}


void ExperimentDesign::setNCols(unsigned cols) {
	_nCols = cols;

	for (auto& row : assignedPlotTypes) {
		while (row.size() < cols) {
			row.append(plotTypes[0]);
		}
		while (row.size() > cols) {
			row.pop_back();
		}
	}
}


void ExperimentDesign::setSize(unsigned rows, unsigned cols) {
	setNCols(cols);
	setNRows(rows);
}


unsigned ExperimentDesign::nRows() const {
	return _nRows;
}


unsigned ExperimentDesign::nCols() const {
	return _nCols;
}

void ExperimentDesign::setPlotType(unsigned row, unsigned col, const PlotType& type) {
	// TODO: Needs exception handling for out of bounds or unregistered plot type
	assignedPlotTypes[row][col] = type;
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
	// +1 because experimentalists don't count from 0
	return QString::number(y * _nRows + x + 1);
}


const ExperimentDesign::PlotType ExperimentDesign::getPlotType(unsigned row, unsigned col) const {
	return assignedPlotTypes[row][col];
}

const QVector<ExperimentDesign::PlotType> ExperimentDesign::getAllPlotTypes() const {
	return plotTypes;
}


void ExperimentDesign::addPlotType(const PlotType& newType) {
	plotTypes.append(newType);
}


void ExperimentDesign::deletePlotType(unsigned index) {
	auto thisPlot = plotTypes[index];

	for (auto& row : assignedPlotTypes) {
		for (unsigned i = 0; i < row.size(); ++i) {
			if (row[i] == thisPlot) {
				row[i] = plotTypes[0];
			}
		}
	}
	
	plotTypes.remove(index);
}


void ExperimentDesign::deletePlotType(const PlotType& toDelete) {
	for (unsigned i = 0; i < plotTypes.size(); ++i) {
		if (plotTypes[i] == toDelete) {
			plotTypes.remove(i);
			break;
		}
	}
}


void ExperimentDesign::editPlotType(unsigned index, const PlotType& newType) {
	auto oldType = plotTypes[index];
	plotTypes[index] = newType;

	for (auto& row : assignedPlotTypes) {
		for (auto& type : row) {
			if (type == oldType) {
				type = newType;
			}
		}
	}
}


bool operator==(const ExperimentDesign::PlotType& left, const ExperimentDesign::PlotType& right) {
	return left.displayColour == right.displayColour && left.name == right.name;
}