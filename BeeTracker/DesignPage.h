#pragma once
#include "PlotTypeModel.h"
#include <QWidget>


class ExperimentDesign;
class QSpinBox;
class QVBoxLayout;
class QGridLayout;
class QTableView;


class DesignPage : public QWidget {
	Q_OBJECT
	friend class ConfigPersistor;

public:
	DesignPage(QWidget* parent = nullptr);
	~DesignPage();

	void displayDesign(const ExperimentDesign& design);

signals:
	void changeCurrentDesignNRows(const unsigned nRows);
	void changeCurrentDesignNCols(const unsigned nCols);

private:
	PlotTypeModel plotTypeModel;
	QTableView* plotTypeList;

	QSpinBox* rowSpinner = nullptr;
	QSpinBox* colSpinner = nullptr;

	QVBoxLayout* layout = nullptr;

	QGridLayout* plotGridLayout = nullptr;

};

