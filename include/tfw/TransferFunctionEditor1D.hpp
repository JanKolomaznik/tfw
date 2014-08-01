#ifndef TRANSFERFUNCTIONEDITOR1D_HPP
#define TRANSFERFUNCTIONEDITOR1D_HPP

#include <array>

#include <QWidget>
#include <QGraphicsScene>
#include <QRadioButton>

#include "tfw/TransferFunctionView.hpp"
#include "tfw/ATransferFunctionEditor.hpp"

namespace Ui {
class TransferFunctionEditor1D;
}

namespace tfw {

template <typename TGraphicsView>
class FreeHandMouseHandler : public TGraphicsView
{
public:
	FreeHandMouseHandler(QWidget *parent = nullptr)
		: TGraphicsView(parent)
		, mCurrentCurve(nullptr)
		, mIsDrawing(false)
	{}

	void
	setEditedCurve(FreeHandCurve *aCurve, int aChannel)
	{
		mCurrentCurve = aCurve;
		mCurrentChannel = aChannel;
	}

	void
	setTransferFunction(TransferFunction1D &aTransferFunction)
	{
		mTransferFunction = &aTransferFunction;
	}

protected:
	/*void
	mouseDoubleClickEvent(QMouseEvent * event) override;*/
	void
	mouseMoveEvent(QMouseEvent * event) override
	{
		if (mIsDrawing) {
			TFW_ASSERT(mTransferFunction);
			QPointF currentPoint = this->mapToScene(event->pos());
			mCurrentCurve->setSegment(currentPoint, mLastPoint);
			mTransferFunction->setChannelRange(currentPoint.x(), currentPoint.y(), mLastPoint.x(), mLastPoint.y(), mCurrentChannel);
			mLastPoint = currentPoint;
			emit this->transferFunctionModified();
		}
	}

	void
	mousePressEvent(QMouseEvent * event) override
	{
		if (mCurrentCurve) {
			mIsDrawing = true;
			mLastPoint = this->mapToScene(event->pos());
		}
	}

	void
	mouseReleaseEvent(QMouseEvent * event) override
	{
		mIsDrawing = false;
	}

	void
	wheelEvent(QWheelEvent * event) override
	{
		QPoint numDegrees = event->angleDelta() / 8;

		if (numDegrees.y() > 0) {
			this->zoom(1.2);
		} else {
			this->zoom(1.0 / 1.2);
		}
		event->accept();
	}

	TransferFunction1D *mTransferFunction;

	FreeHandCurve *mCurrentCurve;
	int mCurrentChannel;

	QPointF mLastPoint;
	bool mIsDrawing;
};

typedef FreeHandMouseHandler<TransferFunctionView> FreeHandTransferFunctionView;


class TransferFunctionEditor1D : public ATransferFunctionEditor
{
	Q_OBJECT
public:
	static const int cChannelCount = 4;

	explicit TransferFunctionEditor1D(QWidget *parent = nullptr);
	~TransferFunctionEditor1D();

	void
	setTransferFunction(TransferFunction1D &aTransferFunction);

private:
	Ui::TransferFunctionEditor1D *ui;
	QGraphicsScene mTFScene;
	QGraphicsRectItem *mBoundingRect;

	std::array<FreeHandCurve, cChannelCount> mCurves;
	std::array<QRadioButton *, cChannelCount> mRadioButtons;
};

} // namespace tfw

#endif // TRANSFERFUNCTIONEDITOR1D_HPP
