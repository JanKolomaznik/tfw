#pragma once

#include <memory>

#include <QWidget>

#include "tfw/data/ATransferFunction.hpp"
#include "tfw/data/AStatistics.hpp"

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
	boundingRect() const override
	{
		return mBoundingBox;
	}

	void
	setColor(QColor aColor)
	{
		mColor = aColor;
	}

	void
	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
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


class Histogram : public QGraphicsItem
{
public:
	Histogram(QRectF aBoundingBox = QRectF(), QGraphicsItem * parent = nullptr)
		: QGraphicsItem(parent)
		, mBoundingBox(aBoundingBox)
	{}

	QRectF
	boundingRect() const override
	{
		return mBoundingBox;
	}

	void
	setColor(QColor aColor)
	{
		mColor = aColor;
	}

	void
	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
	{
		QPen pen(QColor(mColor.red(), mColor.green(), mColor.blue()));
		pen.setWidth(0);
		pen.setCosmetic(true);
		painter->setPen(pen);

		QBrush brush(mColor);
		painter->setBrush(brush);
		//painter->drawPolyline(mSamples.data(), mSamples.size());
		painter->drawPolygon(mSamples.data(), mSamples.size());
	}

	void
	setSamples(std::vector<QPointF> aSamples)
	{
		mSamples = std::move(aSamples);
		mSamples.push_back(QPointF(mSamples.back().x(), 0.0));
		mSamples.push_back(QPointF(mSamples.front().x(), 0.0));
	}

protected:
	QRectF mBoundingBox;
	std::vector<QPointF> mSamples;
	QColor mColor;
};


} // namespace tfw

