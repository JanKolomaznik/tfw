#pragma once

#include <algorithm>

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>

#include <QPointF>
#include <QWheelEvent>

#include "tfw/data/ATransferFunction.hpp"

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
		tfw::insertRange(mPoints, aFirst, aSecond, pointComparator);
		update();
	}

	void
	clear()
	{
		mPoints.clear();
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
signals:
	void
	transferFunctionModified();
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

	void
	drawBackground(QPainter * painter, const QRectF & rect) override
	{
		painter->fillRect(rect, Qt::lightGray);

		QPen gridPen(Qt::black);
		gridPen.setStyle(Qt::DashLine);
		gridPen.setWidth(0);
		gridPen.setCosmetic(true);
		painter->setPen(gridPen);

		double hSections = width() / 30.0;
		double sectionWidth = rect.width() / hSections;
		double step = std::pow(10.0, std::ceil(std::log10(sectionWidth)));
		for (double x = std::ceil(rect.left() / step) * step; x <= rect.right(); x += step) {
			painter->drawLine(x, rect.bottom(), x, rect.top());
		}
	}
private:
};

} // namespace tfw

