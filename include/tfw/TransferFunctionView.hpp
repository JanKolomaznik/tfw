#pragma once

#include <algorithm>

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>

#include <QPointF>
#include <QWheelEvent>

#include "tfw/data/ATransferFunction.hpp"
#include "tfw/GraphicsItems.hpp"

namespace tfw {

class TransferFunctionView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit TransferFunctionView(QWidget *parent = nullptr)
		: QGraphicsView(parent)
	{
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setCacheMode(QGraphicsView::CacheNone);
		scale(1.0, -1.0);

		//setViewportUpdateMode(ViewportUpdateMode::FullViewportUpdate);
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

	QRectF
	currentView(int aWidth, int aHeight) const
	{
		auto srect = sceneRect();
		auto p1 = mapToScene(2, 1);
		auto p2 = mapToScene(aWidth, aHeight);
		p1.setY(srect.top());
		p2.setY(srect.bottom());

		return QRectF(p1, p2);
	}

	void
	resizeEvent(QResizeEvent *event) override
	{
		QRectF rect = currentView(event->oldSize().width(), event->oldSize().height());
		QGraphicsView::resizeEvent(event);
		fitInView(rect);
		update();
	}


	static double
	getGridStep(double aWidth, double aMaxSections)
	{
		static const std::array<int, 4> cMultipliers = { 1, 2, 5, 10 };

		double sectionWidth = aWidth / aMaxSections;

		double initialStep = std::pow(10.0, std::floor(std::log10(sectionWidth)));
		double step = initialStep;
		for (int multiplier : cMultipliers) {
			step = initialStep * multiplier;
			if (step >= sectionWidth) {
				break;
			}
		}
		return step;
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

		QRectF visible = currentView(width(), height());

		double hSections = width() / 30.0;
		double vSections = height() / 30.0;
		double hStep = getGridStep(visible.width(), hSections);
		double vStep = getGridStep(visible.height(), vSections);
		for (double x = std::ceil(rect.left() / hStep) * hStep; x <= rect.right(); x += hStep) {
			painter->drawLine(x, rect.bottom(), x, rect.top());
		}

		for (double y = std::ceil(rect.top() / vStep) * vStep; y <= rect.bottom(); y += vStep) {
			//auto p1 = rect.left();
			//auto p2 = rect.right();

			//painter->drawLine(rect.left(), y, rect.right(), y);
			painter->drawLine(QPointF(rect.right(), y), QPointF(rect.left(), y));
		}
	}
private:
};

} // namespace tfw

