#pragma once

#include <QDial>
#include <QMouseEvent>
#include <QPoint>
#include <QtMath>
#include <QKeyEvent>
#include <QApplication>


class CustomDial : public QDial {
	Q_OBJECT

public:
	explicit CustomDial(QWidget* parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	void customMouseEvent(QMouseEvent* event);
	void updateDialValue(const QPoint& mousePos);
};
