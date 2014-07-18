#pragma once

#include <algorithm>

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>

#include <QPointF>
#include <QWheelEvent>

namespace tfw {


class FreeHandCurve : public QGraphicsItem
{
public:
	FreeHandCurve(QRectF aBoundingBox = QRectF(), QGraphicsItem * parent = nullptr)
		: QGraphicsItem(parent)
		, mBoundingBox(aBoundingBox)
	{}

	void
	setBoundingRect(QRectF aBoundingRect)
	{
		prepareGeometryChange();
		mBoundingBox = aBoundingRect;
	}

	QRectF
	boundingRect() const
	{
		return mBoundingBox;
	}

	void
	setColor(QColor aColor)
	{
		mColor = aColor;
	}

	void
	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
	{
		QPen pen(mColor);
		pen.setWidth(0);
		pen.setCosmetic(true);
		painter->setPen(pen);
		painter->drawPolyline(mPoints.data(), mPoints.size());
	}

	void
	setSegment(QPointF aFirst, QPointF aSecond)
	{
		// TODO - clamp points by linear interpolation
		auto pointComparator = [](const QPointF &aFirst, const QPointF &aSecond) -> bool {
			return aFirst.x() < aSecond.x();
		};
		if (!pointComparator(aFirst, aSecond)) {
			std::swap(aFirst, aSecond);
		}

		auto lower = std::lower_bound(begin(mPoints), end(mPoints), aFirst, pointComparator);
		auto upper = std::upper_bound(lower/*begin(mPoints)*/, end(mPoints), aSecond, pointComparator);

		auto it = mPoints.erase(lower, upper);
		std::vector<QPointF> tmpVec = { aFirst, aSecond };
		mPoints.insert(it, begin(tmpVec), end(tmpVec));
		update();
	}

protected:
	QRectF mBoundingBox;
	std::vector<QPointF> mPoints;
	QColor mColor;
};


class TransferFunctionView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit TransferFunctionView(QWidget *parent = nullptr)
		: QGraphicsView(parent)
	{
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}

public slots :
	void
	zoom(float aZoomFactor)
	{
		scale(aZoomFactor, 1.0);
	}

protected:
	void
	showEvent(QShowEvent * event) override
	{
		QGraphicsView::showEvent(event);
		fitInView(sceneRect());
	}

	/*void
	wheelEvent(QWheelEvent *event) override
	{
		QPoint numDegrees = event->angleDelta() / 8;

		if (numDegrees.y() > 0) {
			zoom(1.2);
		} else {
			zoom(1.0 / 1.2);
		}
		event->accept();
	}*/

	void
	resizeEvent(QResizeEvent *event) override
	{
		auto srect = sceneRect();
		auto p1 = mapToScene(2, 1);
		auto p2 = mapToScene(event->oldSize().width()-2, event->oldSize().height()-1);
		p1.setY(srect.top());
		p2.setY(srect.bottom());

		QRectF rect(p1, p2);
		QGraphicsView::resizeEvent(event);
		fitInView(rect);
	}
private:
};

} // namespace tfw

