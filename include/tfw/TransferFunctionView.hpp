#pragma once

#include <QWidget>
#include <QGraphicsView>

#include <QWheelEvent>

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

	void
	wheelEvent(QWheelEvent *event) override
	{
		QPoint numDegrees = event->angleDelta() / 8;

		if (numDegrees.y() > 0) {
			zoom(1.2);
		} else {
			zoom(1.0 / 1.2);
		}
		event->accept();
	}

	void
	resizeEvent(QResizeEvent *event) override
	{
		QTransform t = viewportTransform();
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

