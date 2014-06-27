#include "include/tfw/TransferFunctionEditor1D.hpp"
#include "ui_TransferFunctionEditor1D.h"

#include <QGraphicsItem>

namespace tfw {


TransferFunctionEditor1D::TransferFunctionEditor1D(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::TransferFunctionEditor1D)
{
	ui->setupUi(this);

	QRectF rect = QRectF(0.0, 0.0, 100, 100);

	mTFScene.addRect(rect, QPen(), QBrush(Qt::red));
	mTFScene.addEllipse(0.0, 100, 5, 5);//->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	mTFScene.addEllipse(100, 100, 5, 5);//->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	mTFScene.addEllipse(50, 50, 5, 5);//->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	mTFScene.addEllipse(20, 70, 5, 5);//->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

	ui->mTransferFunctionView->setScene(&mTFScene);
	//ui->mTransferFunctionView->setSceneRect(0.0, 1.0, 1.0, 1.0);
	ui->mTransferFunctionView->fitInView(rect);

}

TransferFunctionEditor1D::~TransferFunctionEditor1D()
{
	delete ui;
}

} // namespace tfw
