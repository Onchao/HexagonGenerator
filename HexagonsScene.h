#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <vector>

#include "HexagonsGraph.h"
#include "HexView.h"
#include "ResourceManager.h"

class HexagonGenerator;
class LocationLabel;

class HexagonsScene : public QGraphicsScene {
public:
	HexagonsScene(HexagonsGraph* graph, int width, int height, HexagonGenerator* hexagonGenerator, QObject* parent = nullptr);
	~HexagonsScene();
	void updateHexColors();
	void updateTerrain(Terrain type);
	void setLocationLabel(LocationLabel* label);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	void wheelEvent(QGraphicsSceneWheelEvent* event) override;

private:
	HexagonsGraph* hexagonsGraph;
	std::vector<HexView*> hexagonsView;

	bool isRightButtonPressed = false;

	QGraphicsPixmapItem* desertItem;
	QGraphicsPixmapItem* forestItem;
	QGraphicsPixmapItem* plainsItem;
	LocationLabel* locationLabel;
	HexagonGenerator* hexagonGenerator;
};