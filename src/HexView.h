#pragma once

#include <QGraphicsItem>
#include <QBrush>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter.h>
#include "HexagonsGraph.h"

class HexagonGenerator;

class HexView : public QGraphicsPolygonItem
{
public:
	HexView(Hexagon hex, HexagonsGraph* graph, int shiftX, int shiftY, HexagonGenerator* hexagonGenerator);
	void updateHexColor();
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
private:
	Hexagon hexagon;
	HexagonsGraph* hexagonsGraph;
	HexagonGenerator* hexagonGenerator;
};

