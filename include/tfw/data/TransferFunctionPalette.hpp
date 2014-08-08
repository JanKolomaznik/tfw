#pragma once

#include <memory>
#include <vector>

#include <QAbstractListModel>
#include <QPixmap>

#include "tfw/data/ATransferFunction.hpp"

namespace tfw {

class TransferFunctionPalette
{
public:
	int size() const {
		return int(mFunctions.size());
	}

	const ATransferFunction & get(int aIndex) const
	{
		const auto &tf = mFunctions.at(aIndex);
		if (!tf) {
			TFW_THROW(TFWException());
		}
		return *tf;
	}

	ATransferFunction & get(int aIndex)
	{
		auto &tf = mFunctions.at(aIndex);
		if (!tf) {
			TFW_THROW(TFWException());
		}
		return *tf;
	}

	void
	add(std::shared_ptr<ATransferFunction> aTransferFunction)
	{
		if (!aTransferFunction) {
			TFW_THROW(TFWException());
		}
		mFunctions.push_back(aTransferFunction);
	}

protected:
	std::vector<std::shared_ptr<ATransferFunction>> mFunctions;
};



class TransferFunctionPaletteModel : public QAbstractListModel
{
public:

	TransferFunctionPaletteModel()
		: mPalette(std::make_shared<TransferFunctionPalette>())
	{}
	TransferFunctionPaletteModel(std::shared_ptr<TransferFunctionPalette> aPalette)
		: mPalette(aPalette)
	{}
	int
	rowCount(const QModelIndex &parent = QModelIndex()) const override
	{
		return mPalette->size();
	}

	int
	columnCount(const QModelIndex &parent = QModelIndex()) const override
	{
		return 1;
	}

	QVariant
	data(const QModelIndex &aIndex, int aRole = Qt::DisplayRole) const override
	{
		if (aIndex.row() >= rowCount()
			|| aIndex.row() < 0)
		{
			return QVariant();
		}
		switch (aRole) {
		case Qt::DisplayRole:
			return QVariant(mPalette->get(aIndex.row()).name().c_str());
		case Qt::DecorationRole: {
			auto pixmap = QPixmap(64, 64);
			pixmap.fill(Qt::black);
			return pixmap;
		}
		case Qt::TextAlignmentRole:
			return Qt::AlignCenter;
		default:
			return QVariant();
		}
	}

	QVariant
	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
	{
		return QVariant();
	}

	const TransferFunctionPalette &
	palette() const
	{
		return *mPalette;
	}

	TransferFunctionPalette &
	palette()
	{
		return *mPalette;
	}

	QModelIndex
	modelIndexFromId(int aTFId) const
	{
		return index(aTFId, 0);
	}

	void
	add(std::shared_ptr<ATransferFunction> aTransferFunction)
	{

		beginInsertRows(QModelIndex(), int(mPalette->size()), int(mPalette->size()));
		mPalette->add(aTransferFunction);
		endInsertRows();
	}

protected:

	std::shared_ptr<TransferFunctionPalette> mPalette;
};

} // namesapce tfw
