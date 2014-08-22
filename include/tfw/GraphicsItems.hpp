#pragma once

#include <memory>
#include <array>

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QPainter>
#include <QGraphicsScene>

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


class AEditablePrimitive : public QGraphicsObject
{
public:
	AEditablePrimitive(QGraphicsItem * parent = nullptr)
		: QGraphicsObject(parent)
	{}
};


class AManipulator : public QGraphicsObject
{
public:
	AManipulator(QGraphicsItem * parent = nullptr)
		: QGraphicsObject(parent)
		, mIsActivated(false)
	{
		setAcceptHoverEvents(true);

//		setFlag(QGraphicsItem::ItemIsSelectable);
		setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
		setFlag(QGraphicsItem::ItemIsMovable);
		setFlag(QGraphicsItem::ItemIgnoresTransformations);
	}

protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

	/*void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;*/

	bool mIsActivated;
};


class PositionManipulator : public AManipulator
{
	Q_OBJECT;
public:
	PositionManipulator(QGraphicsItem * parent = nullptr)
		: AManipulator(parent)
		, mBoundingRect(-3, -3, 6, 6)
	{
		QObject::connect(this, &QGraphicsObject::xChanged, this, &PositionManipulator::notifyAboutPositionUpdate);
		QObject::connect(this, &QGraphicsObject::yChanged, this, &PositionManipulator::notifyAboutPositionUpdate);
		QObject::connect(this, &QGraphicsObject::zChanged, this, &PositionManipulator::notifyAboutPositionUpdate);
	}

	QRectF
	boundingRect() const override
	{
		return mBoundingRect;
	}

	void
	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
	{
		QPen pen(QColor(0, 255, 0));
		pen.setWidth(0);
		pen.setCosmetic(true);
		painter->setPen(pen);

		if (mIsActivated) {
			QBrush brush(QColor(0, 255, 0));
			painter->setBrush(brush);
		} else {
			QBrush brush(QColor(0, 0, 255));
			painter->setBrush(brush);
		}

		painter->drawEllipse(mBoundingRect);
	}

signals:
	void
	positionUpdated();

protected slots:
	void
	notifyAboutPositionUpdate() {
		emit positionUpdated();
	}

protected:
	/* TODO
	QVariant
	itemChange(GraphicsItemChange change, const QVariant &value) override
	{
		if (change == ItemPositionChange && scene()) {
			// value is the new position.
			QPointF newPos = value.toPointF();
			QRectF rect = scene()->sceneRect();
			if (!rect.contains(newPos)) {
				// Keep the item inside the scene rect.
				newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
				newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
				return newPos;
			}
		}
		return QGraphicsItem::itemChange(change, value);
	}*/

	QRectF mBoundingRect;
};

class EditableRectangle : public AEditablePrimitive
{
public:
	EditableRectangle(QRectF aRect, QGraphicsItem * parent = nullptr)
		: AEditablePrimitive(parent)
		, mRectangle(aRect)
		, mAcceptsManipulation(true)
	{
		setFlag(QGraphicsItem::ItemIsMovable);
		initManipulators();
	}

	QRectF
	boundingRect() const override
	{
		return mRectangle;
	}

	void
	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
	{
		QPen pen(QColor(0, 0, 0));
		pen.setWidth(0);
		pen.setCosmetic(true);
		painter->setPen(pen);

		QBrush brush(QColor(128, 200, 0));
		painter->setBrush(brush);
		painter->drawRect(mRectangle);
	}

protected:
	void
	initManipulators()
	{
		for (int i = 0; i < 4; ++i) {
			mManipulators[i] = new PositionManipulator(this);
		}
		updateManipulatorPositions();

		for (int i = 0; i < 4; ++i) {
			QObject::connect(
				mManipulators[i],
				&PositionManipulator::positionUpdated,
				[this, i]() {
					updateFromManipulator(i);
				});
		}
	}

	void
	updateManipulatorPositions()
	{
		mManipulators[0]->setPos(mRectangle.topLeft());
		mManipulators[1]->setPos(mRectangle.topRight());
		mManipulators[2]->setPos(mRectangle.bottomLeft());
		mManipulators[3]->setPos(mRectangle.bottomRight());
	}

	void
	updateFromManipulator(int aManipulatorIndex)
	{
		if (!mAcceptsManipulation) {
			return;
		}
		auto a0 = mManipulators[0]->pos();
		auto a1 = mManipulators[1]->pos();
		auto a2 = mManipulators[2]->pos();
		auto a3 = mManipulators[3]->pos();
		mAcceptsManipulation = false;
		switch (aManipulatorIndex) {
		case 0:
			mManipulators[1]->setY(mManipulators[aManipulatorIndex]->y());
			mManipulators[2]->setX(mManipulators[aManipulatorIndex]->x());
			break;
		case 1:
			mManipulators[0]->setY(mManipulators[aManipulatorIndex]->y());
			mManipulators[3]->setX(mManipulators[aManipulatorIndex]->x());
			break;
		case 2:
			mManipulators[3]->setY(mManipulators[aManipulatorIndex]->y());
			mManipulators[0]->setX(mManipulators[aManipulatorIndex]->x());
			break;
		case 3:
			mManipulators[2]->setY(mManipulators[aManipulatorIndex]->y());
			mManipulators[1]->setX(mManipulators[aManipulatorIndex]->x());
			break;
		default:
			TFW_ASSERT(false);
			break;
		}
		auto b0 = mManipulators[0]->pos();
		auto b1 = mManipulators[1]->pos();
		auto b2 = mManipulators[2]->pos();
		auto b3 = mManipulators[3]->pos();
		auto minmaxX = std::minmax({ mManipulators[0]->x(), mManipulators[1]->x() });
		auto minmaxY = std::minmax({ mManipulators[0]->y(), mManipulators[2]->y() });
		mRectangle.setLeft(minmaxX.first);
		mRectangle.setRight(minmaxX.second);
		mRectangle.setTop(minmaxY.first);
		mRectangle.setBottom(minmaxY.second);
		mAcceptsManipulation = true;
	}

	QRectF mRectangle;

	std::array<PositionManipulator *, 4> mManipulators;

	bool mAcceptsManipulation;
};


} // namespace tfw

