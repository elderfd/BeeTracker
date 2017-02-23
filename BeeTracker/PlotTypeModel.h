#pragma once
#include "ExperimentDesign.h"
#include <QAbstractTableModel>


class PlotTypeModel : public QAbstractTableModel {
	Q_OBJECT

	friend class DesignPage;
public:
	PlotTypeModel(QObject* parent = 0) : QAbstractTableModel(parent) {}

	void setDesign(ExperimentDesign* design) {
		beginResetModel();
		this->design = design;
		endResetModel();
	}

	int rowCount(const QModelIndex &parent = QModelIndex()) const override {
		if (design != nullptr) {
			return design->plotTypes.size();
		}

		return 0;
	}

	int columnCount(const QModelIndex &parent = QModelIndex()) const override {
		return 2;
	}

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
		if (design != nullptr && index.isValid() && role == Qt::DisplayRole) {
			auto plotType = design->plotTypes[index.row()];

			if (index.column() == 0) {
				return plotType.name;
			} else {
				return plotType.displayColour;
			}
		}

		return QVariant();
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
		if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
			if (section == 0) {
				return "Name";
			} else {
				return "Display Colour";
			}
		}
		return QVariant();
	}
private:
	ExperimentDesign* design = nullptr;
};