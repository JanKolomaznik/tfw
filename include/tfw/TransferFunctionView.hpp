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
	{}

protected:
	void
	wheelEvent(QWheelEvent *event) override
	{
		QPoint numDegrees = event->angleDelta() / 8;

		if (numDegrees.y() > 0) {
			scale(1.2, 1.0);
		} else {
			scale(1.0 / 1.2, 1.0);
		}
		event->accept();
	}

	void
	resizeEvent(QResizeEvent *event) override
	{
		static bool once = true;
		auto rect = sceneRect();
		QGraphicsView::resizeEvent(event);
		if (once) {
			fitInView(rect);
			once = false;
		}
	}
private:
};

} // namespace tfw

