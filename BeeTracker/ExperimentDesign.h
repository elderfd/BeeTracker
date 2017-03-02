#pragma once
#include <QColor>
#include <QString>
#include <QVector>

namespace pugi {
	class xml_node;
}


class ExperimentDesign {
public:
	ExperimentDesign();
	~ExperimentDesign();

	class PlotType {
	public:
		QString name;
		QColor displayColour;

		friend bool operator==(const ExperimentDesign::PlotType& left, const ExperimentDesign::PlotType& right);
	};

	void setNRows(unsigned rows);
	void setNCols(unsigned cols);
	void setSize(unsigned rows, unsigned cols);

	void setPlotType(unsigned row, unsigned col, const PlotType& type);
	const PlotType getPlotType(unsigned row, unsigned col) const;

	const QVector<PlotType> getAllPlotTypes() const;
	void addPlotType(const PlotType& newType);
	void deletePlotType(const PlotType& toDelete);
	void deletePlotType(unsigned index);
	void editPlotType(unsigned index, const PlotType& newType);

	unsigned nRows() const;
	unsigned nCols() const;

	friend pugi::xml_node& operator<<(pugi::xml_node& el, const ExperimentDesign& design);
	friend pugi::xml_node& operator>>(pugi::xml_node& el, ExperimentDesign& design);

	QString xyToPlotID(unsigned int x, unsigned int y) const;

	static const unsigned defaultNRows = 5;
	static const unsigned defaultNCols = 5;
	static const unsigned maxSize = 10;
	static const PlotType defaultPlotType;
private:
	unsigned _nRows = 0;
	unsigned _nCols = 0;

	QVector<QVector<PlotType>> assignedPlotTypes;
	QVector<PlotType> plotTypes;
};

