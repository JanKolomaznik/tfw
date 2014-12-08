#include "tfw/TransferFunctionEditor2D.hpp"
#include "tfw/ColorChooserButton.hpp"

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

	auto button = new ColorChooserButton(this);
	toolbar->addWidget(button);
	button->setColor(QColor(0, 0, 255, 150));
	ui->mTransferFunctionView->setCurrentColor(button->color());
	button->enableAlpha(true);
	QObject::connect(
		button,
		&ColorChooserButton::colorUpdated,
		[this, button]() {
			ui->mTransferFunctionView->setCurrentColor(button->color());
		});

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
	QRectF rect = QRectF(
			aTransferFunction.range().first[0],
			aTransferFunction.range().first[1],
			aTransferFunction.range().second[0] - aTransferFunction.range().first[0],
			aTransferFunction.range().second[1] - aTransferFunction.range().first[1]);
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

	if (mStatistics && mStatistics->hasScatterPlot()) {
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
			auto r = rectangle->boundingRect();
			auto topLeft = rectangle->pos() + r.topLeft();
			auto bottomRight = rectangle->pos() + r.bottomRight();
			auto tfColor = TransferFunction2D::Color {
					rectangle->color().redF(),
					rectangle->color().greenF(),
					rectangle->color().blueF(),
					rectangle->color().alphaF(),
				};
			mTransferFunction->setColor(
				TransferFunction2D::RangePoint{ float(topLeft.x()), float(topLeft.y()) },
				TransferFunction2D::RangePoint{ float(bottomRight.x()), float(bottomRight.y()) },
				tfColor);
		}
	}


	emit transferFunctionModified();
}

} // namespace tfw
