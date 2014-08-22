#pragma once

#include <algorithm>

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QScrollBar>

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
		setCacheMode(QGraphicsView::CacheNone);
		scale(1.0, -1.0);

		setViewportUpdateMode(ViewportUpdateMode::FullViewportUpdate);
	}

public slots :
	void
	zoom(float aZoomFactorX, float aZoomFactorY = 1.0f)
	{
		scale(aZoomFactorX, aZoomFactorY);
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

	void
	resizeEvent(QResizeEvent *event) override
	{
		QRect oldView(0, 0, event->oldSize().width(), event->oldSize().height());
		QRectF currentView = mapToScene(oldView).boundingRect();
		if (currentView.isNull()) {
			currentView = sceneRect();
		}
		//auto horizontalScroll = horizontalScrollBar()->value();
		//auto verticalScroll = verticalScrollBar()->value();
		//QPointF center = mapToScene(oldView.center());
		if (currentView.isValid()) {
			double sx = event->size().width() / currentView.width();
			double sy = event->size().height() / currentView.height();
			setTransform(QTransform::fromScale(sx, -sy));
			/*horizontalScrollBar()->setValue(horizontalScroll);
			verticalScrollBar()->setValue(verticalScroll);*/
		}
		QGraphicsView::resizeEvent(event);
		centerOn(currentView.center());
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
		static const int cTextMargin = 2;
		static const int cTextSize = 10;
		static const int cLineDistance = 60;
		QFont font = painter->font();
		font.setPointSize(8);
		painter->setFont(font);
		const QFontMetrics &metrics = painter->fontMetrics();

		painter->fillRect(rect, Qt::lightGray);

		QPen gridPen(Qt::black);
		gridPen.setStyle(Qt::DashLine);
		gridPen.setWidth(0);
		gridPen.setCosmetic(true);
		painter->setPen(gridPen);

		QRectF visible = mapToScene(viewport()->rect()).boundingRect();

		double hSections = double(width()) / cLineDistance;
		double vSections = double(height()) / cLineDistance;
		double hStep = getGridStep(visible.width(), hSections);
		double vStep = getGridStep(visible.height(), vSections);

		double fromX = std::max(rect.left(), sceneRect().left());
		double toX = std::min(sceneRect().right(), rect.right());
		double startX = std::ceil(fromX / hStep) * hStep;

		double fromY = std::max(rect.top(), sceneRect().top());
		double toY = std::min(sceneRect().bottom(), rect.bottom());
		double startY = std::ceil(fromY / vStep) * vStep;
		for (double x = startX; x <= toX; x += hStep) {
			painter->drawLine(QPointF(x, fromY), QPointF(x, toY));

			painter->setWorldMatrixEnabled(false);
				QPoint textAnchor = mapFromScene(x, fromY);
				QString text = QString::number(x, 'g', 4);
				QRect textRect(textAnchor.x() - cLineDistance / 2, textAnchor.y() - cTextMargin - cTextSize, cLineDistance, cTextSize);
				textRect = metrics.boundingRect(textRect, Qt::AlignCenter | Qt::AlignBottom, text);
				painter->fillRect(textRect, Qt::lightGray);
				painter->drawText(textRect, Qt::AlignCenter | Qt::AlignBottom, text);
			painter->setWorldMatrixEnabled(true);
		}

		for (double y = startY; y <= toY; y += vStep) {
			painter->drawLine(QPointF(fromX, y), QPointF(toX, y));

			painter->setWorldMatrixEnabled(false);
				QPoint textAnchor = mapFromScene(fromX, y);
				QString text = QString::number(y, 'g', 4);
				QRect textRect(textAnchor.x() +cTextMargin, textAnchor.y() - cTextSize / 2, cLineDistance, cTextSize);
				textRect = metrics.boundingRect(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
				painter->fillRect(textRect, Qt::lightGray);
				painter->drawText(textRect, Qt::AlignCenter | Qt::AlignBottom, text);
			painter->setWorldMatrixEnabled(true);
		}
	}
private:
};

} // namespace tfw

