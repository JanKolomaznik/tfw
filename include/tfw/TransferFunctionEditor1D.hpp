#ifndef TRANSFERFUNCTIONEDITOR1D_HPP
#define TRANSFERFUNCTIONEDITOR1D_HPP

#include <array>

#include <QWidget>
#include <QGraphicsScene>
#include <QRadioButton>

#include "tfw/TransferFunctionView.hpp"

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
	setEditedCurve(FreeHandCurve *aCurve)
	{
		mCurrentCurve = aCurve;
	}

protected:
	/*void
	mouseDoubleClickEvent(QMouseEvent * event) override;*/
	void
	mouseMoveEvent(QMouseEvent * event) override
	{
		if (mIsDrawing) {
			QPointF currentPoint = this->mapToScene(event->pos());
			mCurrentCurve->setSegment(currentPoint, mLastPoint);
			mLastPoint = currentPoint;
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


	FreeHandCurve *mCurrentCurve;
	QPointF mLastPoint;
	bool mIsDrawing;
};

typedef FreeHandMouseHandler<TransferFunctionView> FreeHandTransferFunctionView;

class TransferFunctionEditor1D : public QWidget
{
	Q_OBJECT
public:
	static const int cChannelCount = 4;

	explicit TransferFunctionEditor1D(QWidget *parent = 0);
	~TransferFunctionEditor1D();

private:
	Ui::TransferFunctionEditor1D *ui;
	QGraphicsScene mTFScene;

	std::array<FreeHandCurve, cChannelCount> mCurves;
	std::array<QRadioButton *, cChannelCount> mRadioButtons;
};

} // namespace tfw

#endif // TRANSFERFUNCTIONEDITOR1D_HPP
