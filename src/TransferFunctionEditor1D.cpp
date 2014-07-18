#include "include/tfw/TransferFunctionEditor1D.hpp"
#include "ui_TransferFunctionEditor1D.h"

#include <QGraphicsItem>
#include <QRadioButton>

namespace tfw {


TransferFunctionEditor1D::TransferFunctionEditor1D(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::TransferFunctionEditor1D)
{
	ui->setupUi(this);

	QRectF rect = QRectF(0.0, 0.0, 100, 100);
	mTFScene.addRect(rect, QPen(), QBrush(Qt::white));

	std::array<QColor, cChannelCount> colors = {
		QColor(255, 0, 0),
		QColor(0, 255, 0),
		QColor(0, 0, 255),
		QColor(0, 0, 0)
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
			ui->mTransferFunctionView->setEditedCurve(&(mCurves[i]));
		});
	}

	//mTFScene.addEllipse(0.0, 100, 5, 5);//->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	//mTFScene.addEllipse(100, 100, 5, 5);//->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	//mTFScene.addEllipse(50, 50, 5, 5);//->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	//mTFScene.addEllipse(20, 70, 5, 5);//->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

	ui->mTransferFunctionView->setScene(&mTFScene);
	ui->mTransferFunctionView->setSceneRect(rect);
	ui->mTransferFunctionView->fitInView(rect);
}

TransferFunctionEditor1D::~TransferFunctionEditor1D()
{
	delete ui;
}

} // namespace tfw
