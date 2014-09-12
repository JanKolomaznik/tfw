#include "include/tfw/TransferFunctionEditor2D.hpp"
#include "ui_TransferFunctionEditor2D.h"

#include <QGraphicsItem>
#include <QRadioButton>
#include <QToolBar>
#include <QBoxLayout>
#include <QAction>
#include <QActionGroup>

namespace tfw {


TransferFunctionEditor2D::TransferFunctionEditor2D(QWidget *parent)
	: ATransferFunctionEditor(parent)
	, ui(new Ui::TransferFunctionEditor2D)
	, mTransferFunction(nullptr)
	, mScatterPlot(nullptr)
{
	ui->setupUi(this);

	QRectF rect = QRectF(0.0, 0.0, 100, 100);
	QPen pen(Qt::black);
	pen.setWidth(0);
	pen.setCosmetic(true);
	mBoundingRect = mTFScene.addRect(rect, pen, QBrush(Qt::NoBrush));

	ui->mTransferFunctionView->setScene(&mTFScene);

	QToolBar *toolbar = new QToolBar(this);
	static_cast<QBoxLayout *>(layout())->insertWidget(0, toolbar);

	auto manipulationModes = new QActionGroup(this);
	QAction *action = toolbar->addAction(tr("Edit"));
	action->setCheckable(true);
	manipulationModes->addAction(action);
	QObject::connect(action, &QAction::toggled, [this](bool aToggled) { ui->mTransferFunctionView->setIsAdding(!aToggled);});

	action = toolbar->addAction(tr("Rectangle"));
	action->setCheckable(true);
	manipulationModes->addAction(action);
	QObject::connect(action, &QAction::toggled, [this](bool aToggled) { ui->mTransferFunctionView->setIsAdding(aToggled);});

	manipulationModes->setExclusive(true);


	QObject::connect(ui->mBakeButton, &QPushButton::clicked, this, &TransferFunctionEditor2D::fillTransferFunction);
}

TransferFunctionEditor2D::~TransferFunctionEditor2D()
{
	delete ui;
}

void
TransferFunctionEditor2D::setTransferFunction(TransferFunction2D &aTransferFunction)
{
	mTransferFunction = &aTransferFunction;
	QRectF rect = QRectF(aTransferFunction.range().first[0], aTransferFunction.range().first[1], aTransferFunction.range().second[0], aTransferFunction.range().second[1]);
	/*for (size_t i = 0; i < cChannelCount; ++i) {
		mCurves[i].setBoundingRect(rect);
		mCurves[i].clear();
	}

	ui->mTransferFunctionView->setTransferFunction(aTransferFunction);*/

	mBoundingRect->setRect(rect);
	ui->mTransferFunctionView->setSceneRect(rect);
	ui->mTransferFunctionView->fitInView(rect);
}

void
TransferFunctionEditor2D::setStatistics(std::shared_ptr<AStatistics> aStatistics)
{
	if (mScatterPlot) {
		delete mScatterPlot;
	}
	mStatistics = aStatistics;

	if (mStatistics) {
		auto data = mStatistics->getScatterPlot();
		mScatterPlot = new ScatterPlot(data.first, std::move(data.second), scatterPlotScaleFactor());
		mTFScene.addItem(mScatterPlot);
		updateScatterPlotSettings();
	}
}

double TransferFunctionEditor2D::scatterPlotScaleFactor() const
{
	return pow(10.0, double(ui->mScatterPlotContrastSlider->value()) / 1000);
}

void TransferFunctionEditor2D::updateScatterPlotSettings()
{
	if (mScatterPlot) {
		mScatterPlot->enableLogScale(ui->mLogScaleCheckBox->checkState() == Qt::Checked);
		mScatterPlot->setScaleFactor(scatterPlotScaleFactor());
	}
}

void TransferFunctionEditor2D::fillTransferFunction()
{
	mTransferFunction->reset();
	auto items = mTFScene.items();
	for (auto item : items) {
		EditableRectangle *rectangle = qgraphicsitem_cast<EditableRectangle *>(item);
		if (rectangle) {

		}
	}


	emit transferFunctionModified();
}

} // namespace tfw
