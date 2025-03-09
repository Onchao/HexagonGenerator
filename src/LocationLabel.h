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
#include <QFontDatabase>

#include "ResourceManager.h"
#include "LocationCard.h"


class LocationLabel : public QFrame
{
	Q_OBJECT

public:
	LocationLabel(QGraphicsScene* scene);
	~LocationLabel();
	LocationLabel(QGraphicsScene* scene, LocationLabel* other, int targetWidthPx, bool simple);
	std::string printTransform();
	void setTransform(float posX, float posY, int size, float rotation);

	void updateLabelName(const QString& name);
	void updateLabelGold(int gold);
	void updateLabelKnowledge(int knowledge);
	void updateLabelPopulation(int population);
	void updateLabelDefence(int defence);

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
	const double mult = std::sqrt(1.2); // because we have already made some locations with 0.38 * 1.2 size
	const double scales[5] = { 0.038,
							  0.038 * mult,
						      0.038 * mult * mult,
							  0.038 * mult * mult * mult,
							  0.038 * mult * mult * mult * mult};
	int currentSize = 0;
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
