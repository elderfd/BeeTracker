#pragma once
#include <QAbstractItemDelegate>
#include <QPainter>
#include <QColorDialog>


class ColourDelegate : public QAbstractItemDelegate {
	Q_OBJECT

public:
	ColourDelegate(QObject* parent = 0) : QAbstractItemDelegate(parent) {};

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
		painter->fillRect(option.rect, index.model()->data(index, Qt::DisplayRole).value<QColor>());
	}

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
		auto editor = new QColorDialog(parent);

		return editor;
	}

	void setEditorData(QWidget* editor, const QModelIndex& index) const override {
		auto value = index.model()->data(index, Qt::EditRole).value<QColor>();

		static_cast<QColorDialog*>(editor)->setCurrentColor(value);
	}

	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override {
		model->setData(index, static_cast<QColorDialog*>(editor)->selectedColor(), Qt::EditRole);
	}

	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
		return QSize(25, 25);
	}
};