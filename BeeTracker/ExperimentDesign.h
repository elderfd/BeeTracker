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
	};

	void setNRows(unsigned rows);
	void setNCols(unsigned cols);

	unsigned nRows() const;
	unsigned nCols() const;

	friend pugi::xml_node& operator<<(pugi::xml_node& el, const ExperimentDesign& design);
	friend pugi::xml_node& operator>>(pugi::xml_node& el, ExperimentDesign& design);

	QString xyToPlotID(unsigned int x, unsigned int y) const;

	QVector<PlotType> plotTypes;

	static const unsigned defaultNRows = 5;
	static const unsigned defaultNCols = 5;
	static const unsigned maxSize = 10;
	static const PlotType defaultPlotType;
private:
	unsigned _nRows = 0;
	unsigned _nCols = 0;
};

