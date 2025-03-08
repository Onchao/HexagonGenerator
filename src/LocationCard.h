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
#include <QFontDatabase>

#include "ResourceManager.h"

class LocationLabel;

class LocationCard : public QFrame
{
	Q_OBJECT

public:
	LocationCard();
	~LocationCard();
	LocationCard(LocationLabel* locationLabel, int targetPixmapSize);

	void scaleEverything(float scale);

private:
	QGraphicsScene labelScene;
	QGraphicsPixmapItem* backgroundItem;
	QLabel* nameLabel;
	QLabel* goldLabel;
	QLabel* knowledgeLabel;
	QLabel* populationLabel;
	QLabel* defenceLabel;
	QGraphicsView* graphicsView;
};
