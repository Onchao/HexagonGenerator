#include "CustomDial.h"

CustomDial::CustomDial(QWidget* parent) : QDial(parent)
{
	setMinimum(0);
	setMaximum(360);
	setFixedSize(60, 60);
	move(208, 434);
	setWrapping(true);
	setValue(0);
	setNotchesVisible(true);
	setNotchTarget(15); // report bug in the documentation about the QDial that these are not pixels but degrees
}

void CustomDial::mousePressEvent(QMouseEvent* event) {
	customMouseEvent(event);
}

void CustomDial::mouseMoveEvent(QMouseEvent* event) {
	customMouseEvent(event);
}

void CustomDial::mouseReleaseEvent(QMouseEvent* event) {
	customMouseEvent(event);
}

void CustomDial::customMouseEvent(QMouseEvent* event)
{
	if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
		updateDialValue(event->pos());
	}
	else {
		QDial::mouseMoveEvent(event);
	}
}

void CustomDial::updateDialValue(const QPoint& mousePos) {
	int step = 15;
	QPoint center = rect().center();
	qreal dx = mousePos.x() - center.x();
	qreal dy = center.y() - mousePos.y();
	qreal angle = -qAtan2(dy, dx) * 180.0 / M_PI - 90.0;
	setValue(static_cast<int>(std::round(angle / step)) * step);
}
