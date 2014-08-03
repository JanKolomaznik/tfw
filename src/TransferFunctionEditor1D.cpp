#include "include/tfw/TransferFunctionEditor1D.hpp"
#include "ui_TransferFunctionEditor1D.h"

#include <QGraphicsItem>
#include <QRadioButton>

namespace tfw {


TransferFunctionEditor1D::TransferFunctionEditor1D(QWidget *parent)
	: ATransferFunctionEditor(parent)
	, ui(new Ui::TransferFunctionEditor1D)
{
	ui->setupUi(this);

	QRectF rect = QRectF(0.0, 0.0, 100, 100);
	//mBoundingRect = mTFScene.addRect(rect, QPen(Qt::white), QBrush(Qt::white));

	std::array<QColor, cChannelCount> colors = {
		QColor(255, 0, 0),
		QColor(0, 255, 0),
		QColor(0, 0, 255),
		QColor(255, 255, 255)
	};
	mRadioButtons[0] = ui->mRedRadioButton;
	mRadioButtons[1] = ui->mGreenRadioButton;
	mRadioButtons[2] = ui->mBlueRadioButton;
	mRadioButtons[3] = ui->mAlphaRadioButton;
	for (size_t i = 0; i < cChannelCount; ++i) {
		mCurves[i].setBoundingRect(rect);
		mCurves[i].setColor(colors[i]);
		mTFScene.addItem(&(mCurves[i]));
		QObject::connect(mRadioButtons[i], &QRadioButton::clicked, [i, this]() {
			ui->mTransferFunctionView->setEditedCurve(&(mCurves[i]), i);
		});
	}

	//mTFScene.addEllipse(0.0, 100, 5, 5);//->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	//mTFScene.addEllipse(100, 100, 5, 5);//->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	//mTFScene.addEllipse(50, 50, 5, 5);//->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	//mTFScene.addEllipse(20, 70, 5, 5);//->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

	ui->mTransferFunctionView->setScene(&mTFScene);

	QObject::connect(
		ui->mTransferFunctionView,
		&TransferFunctionView::transferFunctionModified,
		this,
		&TransferFunctionEditor1D::transferFunctionModified);
}

TransferFunctionEditor1D::~TransferFunctionEditor1D()
{
	delete ui;
}

void
TransferFunctionEditor1D::setTransferFunction(TransferFunction1D &aTransferFunction)
{
	QRectF rect = QRectF(aTransferFunction.range().first, 0.0, aTransferFunction.range().second, 1.0);
	for (size_t i = 0; i < cChannelCount; ++i) {
		mCurves[i].setBoundingRect(rect);
		mCurves[i].clear();
	}

	ui->mTransferFunctionView->setTransferFunction(aTransferFunction);

	//mBoundingRect->setRect(rect);
	ui->mTransferFunctionView->setSceneRect(rect);
	ui->mTransferFunctionView->fitInView(rect);
}

} // namespace tfw
