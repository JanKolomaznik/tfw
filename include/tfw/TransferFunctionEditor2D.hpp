#pragma once

#include <array>

#include <QWidget>
#include <QGraphicsScene>
#include <QRadioButton>

#include "tfw/TransferFunctionView.hpp"
#include "tfw/ATransferFunctionEditor.hpp"
#include "tfw/data/AStatistics.hpp"
#include "tfw/GraphicsItems.hpp"

namespace Ui {
class TransferFunctionEditor2D;
}

namespace tfw {

template <typename TGraphicsView>
class TransferFunction2DZoomHandler : public TGraphicsView
{
public:
	TransferFunction2DZoomHandler(QWidget *parent = nullptr)
		: TGraphicsView(parent)
	{
		this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	}
protected:
	void
	wheelEvent(QWheelEvent * event) override
	{
		QPoint numDegrees = event->angleDelta() / 8;
		float zoomFactor = numDegrees.y() > 0 ? 1.2f : (1.0f / 1.2f);

		if (event->modifiers() & Qt::ControlModifier) {
			this->zoom(1.0f, zoomFactor);
		} else {
			this->zoom(zoomFactor, 1.0f);
		}
		event->accept();
	}
};

template <typename TGraphicsView>
class TransferFunction2DAddRectangleHandler : public TGraphicsView
{
public:
	TransferFunction2DAddRectangleHandler(QWidget *parent = nullptr)
		: TGraphicsView(parent)
		, mIsAdding(false)
	{}

	void
	setTransferFunction(TransferFunction2D &aTransferFunction)
	{
		mTransferFunction = &aTransferFunction;
	}
	void
	setIsAdding(bool aEnable) {
		mIsAdding = aEnable;
	}

protected:
	/*void
	mouseMoveEvent(QMouseEvent * event) override
	{
		if (mIsDrawing) {
			TFW_ASSERT(mTransferFunction);
			QPointF currentPoint = this->mapToScene(event->pos());
			currentPoint.setX(std::min<float>(mTransferFunction->range().second, std::max<float>(mTransferFunction->range().first, currentPoint.x())));
			currentPoint.setY(std::min(1.0, std::max(0.0, currentPoint.y())));

			mCurrentCurve->setSegment(currentPoint, mLastPoint);
			mTransferFunction->setChannelRange(currentPoint.x(), currentPoint.y(), mLastPoint.x(), mLastPoint.y(), mCurrentChannel);
			mLastPoint = currentPoint;
			emit this->transferFunctionModified();
		}
	}*/

	void
	mousePressEvent(QMouseEvent * event) override
	{
		if (mIsAdding) {
			QRectF rect = this->mapToScene(QRect(event->pos(), QSize(100, 100))).boundingRect();
			this->scene()->addItem(new EditableRectangle(rect));
		} else {
			TGraphicsView::mousePressEvent(event);
		}
	}

	/*void
	mouseReleaseEvent(QMouseEvent * event) override
	{
		mIsDrawing = false;
	}*/

	TransferFunction2D *mTransferFunction;


	bool mIsAdding;

};


typedef TransferFunction2DAddRectangleHandler<TransferFunction2DZoomHandler<TransferFunctionView>> TransferFunction2DView;

class TransferFunctionEditor2D : public ATransferFunctionEditor
{
	Q_OBJECT
public:
	static const int cChannelCount = 4;

	explicit TransferFunctionEditor2D(QWidget *parent = nullptr);
	~TransferFunctionEditor2D();

	void
	setTransferFunction(TransferFunction2D &aTransferFunction);

	void
	setStatistics(std::shared_ptr<AStatistics> aStatistics) override;

	QString
	tfName() const override
	{
		if (mTransferFunction) {
			return QString::fromStdString(mTransferFunction->name());
		}
		return QString();
	}

	double scatterPlotScaleFactor() const;

public slots:
	void
	updateScatterPlotSettings();
private:
	Ui::TransferFunctionEditor2D *ui;
	QGraphicsScene mTFScene;
	QGraphicsRectItem *mBoundingRect;

	TransferFunction2D *mTransferFunction;

	std::shared_ptr<AStatistics> mStatistics;
	ScatterPlot *mScatterPlot;
};

} // namespace tfw

