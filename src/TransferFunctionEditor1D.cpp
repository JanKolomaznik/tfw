#include "include/tfw/TransferFunctionEditor1D.hpp"
#include "ui_TransferFunctionEditor1D.h"

#include <QGraphicsItem>
#include <QRadioButton>

namespace tfw {


TransferFunctionEditor1D::TransferFunctionEditor1D(QWidget *parent)
	: ATransferFunctionEditor(parent)
	, ui(new Ui::TransferFunctionEditor1D)
	, mHistogram(nullptr)
{
	ui->setupUi(this);

	QRectF rect = QRectF(0.0, 0.0, 100, 100);
	QPen pen(Qt::black);
	pen.setWidth(0);
	pen.setCosmetic(true);
	mBoundingRect = mTFScene.addRect(rect, pen, QBrush(Qt::NoBrush));

	ui->mTransferFunctionView->setScene(&mTFScene);

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
	mTransferFunction = &aTransferFunction;
	QRectF rect = QRectF(aTransferFunction.range().first, 0.0, aTransferFunction.range().second, 1.0);
	for (size_t i = 0; i < cChannelCount; ++i) {
		mCurves[i].setBoundingRect(rect);
		mCurves[i].clear();
	}

	ui->mTransferFunctionView->setTransferFunction(aTransferFunction);

	mBoundingRect->setRect(rect);
	ui->mTransferFunctionView->setSceneRect(rect);
	ui->mTransferFunctionView->fitInView(rect);
}

void TransferFunctionEditor1D::setStatistics(std::shared_ptr<AStatistics> aStatistics)
{
	if (mHistogram) {
		delete mHistogram;
	}
	mStatistics = aStatistics;

	if (mStatistics) {
		auto range = mStatistics->getHistogramRange();

		mHistogram = new Histogram(QRectF(range.first, 0.0, range.second - range.first, 1.0));
		mTFScene.addItem(mHistogram);
		mHistogram->setColor(QColor(200, 100, 200, 150));
		mHistogram->setSamples(mStatistics->getHistogramSamples());
	}
}

} // namespace tfw
