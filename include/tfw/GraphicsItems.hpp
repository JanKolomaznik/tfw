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

inline QColor
interpolate(const QColor &aC1, const QColor &aC2, double aAlpha)
{
	return QColor::fromRgbF(
			aAlpha * aC2.redF() + (1.0 - aAlpha) * aC1.redF(),
			aAlpha * aC2.greenF() + (1.0 - aAlpha) * aC1.greenF(),
			aAlpha * aC2.blueF() + (1.0 - aAlpha) * aC1.blueF(),
			aAlpha * aC2.alphaF() + (1.0 - aAlpha) * aC1.alphaF());
}

inline QColor
heatColorRamp(double aValue)
{
	static const std::vector<QColor> cColors {
		QColor(0, 128, 128, 0),
		QColor(0, 255, 255, 140),
		QColor(0, 128, 255, 200),
		QColor(0, 255, 0, 230),
		QColor(255, 255, 0, 230),
		QColor(255, 0, 0, 230)
	};
	static const double cStep = 1.0 / (cColors.size() - 1);
	if (aValue <= 0.0) {
		return cColors.front();
	}
	if (aValue >= 1.0) {
		return cColors.back();
	}

	int segment = std::floor(aValue / cStep);
	const QColor &c1 = cColors[segment];
	const QColor &c2 = cColors[segment + 1];
	double alpha = (aValue - segment * cStep) / cStep;
	return interpolate(c1, c2, alpha);
}

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
	appendPoint(QPointF aPoint)
	{
		//TODO - prevent misuse
		mPoints.push_back(aPoint);
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

class ScatterPlot : public QGraphicsItem
{
public:
	ScatterPlot(QRectF aRegion, ScatterPlotData aData, double aScaleFactor, QGraphicsItem * parent = nullptr)
		: QGraphicsItem(parent)
		, mUseLogScale(false)
	{
		setData(aRegion, std::move(aData));
		setScaleFactor(aScaleFactor);
	}

	QRectF
	boundingRect() const override
	{
		return mBoundingBox;
	}

	void
	setData(QRectF aRegion, ScatterPlotData aData)
	{
		mBoundingBox = aRegion;
		mData = std::move(aData);
	}

	void
	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
	{
		QImage image(mData.size[0], mData.size[1], QImage::Format_ARGB32);

		/*computeImage(
			image,
			[this](double &aValue) {
				int val = std::min<int>(250, log(double(aValue) * mScaleFactor + 1));
				return (val << 24) + (val << 16) + val;
			});*/

		if (mUseLogScale) {
			computeImage(
				image,
				[this](const double &aValue) -> int{
					return heatColorRamp(0.00000001 * log(1 + 100000000*aValue) * mScaleFactor).rgba();
				});
		} else {
			computeImage(
				image,
				[this](const double &aValue) -> int{
					return heatColorRamp(aValue * mScaleFactor).rgba();
				});
		}

		painter->drawImage(mBoundingBox, image);
	}

	void
	setScaleFactor(double aScaleFactor)
	{
		mScaleFactor = aScaleFactor;
		update();
	}

	void
	enableLogScale(bool aEnable)
	{
		mUseLogScale = aEnable;
		update();
	}

protected:

	template <typename TOperator>
	void
	computeImage(QImage &aImage, TOperator aOperator) const
	{
		for (int j = 0; j < mData.size[1]; ++j) {
			for (int i = 0; i < mData.size[0]; ++i) {
				aImage.setPixel(i, j, aOperator(mData.buffer[i + j*mData.size[0]]));
			}
		}
	}

	QRectF mBoundingBox;
	ScatterPlotData mData;
	double mScaleFactor;

	bool mUseLogScale;
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
	// qgraphicsitem_cast uses this
	enum { Type = UserType + 100 };
	int type() const override { return Type; }

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
		mAcceptsManipulation = false; //TODO RAII
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
		auto minmaxX = std::minmax({ mManipulators[0]->x(), mManipulators[1]->x() });
		auto minmaxY = std::minmax({ mManipulators[0]->y(), mManipulators[2]->y() });
		prepareGeometryChange();
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

