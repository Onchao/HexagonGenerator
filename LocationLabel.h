#pragma once

#include <QGraphicsItemGroup>
#include <QGraphicsEllipseItem>
#include <QBrush.h>
#include <QPen.h>
#include <QFont.h>
#include <QGraphicsScene.h>
#include <QFrame.h>
#include <QGraphicsProxyWidget>
#include <QtWidgets/QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <iostream>
#include <QPalette>


#include "ResourceManager.h"
#include "LocationCard.h"


class LocationLabel : public QFrame
{
	Q_OBJECT

public:
	LocationLabel(QGraphicsScene* scene);
	~LocationLabel();
	LocationLabel(QGraphicsScene* scene, LocationLabel* other, int targetWidthPx, bool simple);

	void updateText(const QString& name, int gold, int knowledge, int population, int defence);
	void setRotation(int angle);
	void showCenterDot(bool visible);
	void showLabel(bool visible);
	void setSize(int size);
	void scaleEverything(float targetWidthPx);
	void setCenterPosition(QPointF pos);
	std::string printInfo();

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	constexpr static int centerDotSize = 6;
	constexpr static float scales[] = { 0.040, 0.048, 0.058, 0.069, 0.083 };

	float currentScale;
	QGraphicsScene labelScene;
	QGraphicsEllipseItem* centerDot;
	QGraphicsPixmapItem* backgroundItem;
	QGraphicsProxyWidget* proxy;
	QLabel* nameLabel;
	QLabel* goldLabel;
	QLabel* knowledgeLabel;
	QLabel* populationLabel;
	QLabel* defenceLabel;
	QGraphicsView* graphicsView;

	friend LocationCard::LocationCard(LocationLabel* locationLabel, int targetPixmapSize);
};
