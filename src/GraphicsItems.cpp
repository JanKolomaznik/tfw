
#include "tfw/GraphicsItems.hpp"

namespace tfw {


void
AManipulator::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	mIsActivated = true;
	QGraphicsItem::hoverEnterEvent(event);
}

void
AManipulator::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	mIsActivated = false;
	QGraphicsItem::hoverLeaveEvent(event);
}
/*
void
AManipulator::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mousePressEvent(event);
}

void
AManipulator::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent(event);
}

void
AManipulator::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseMoveEvent(event);
}
*/
}  // namespace tfw
