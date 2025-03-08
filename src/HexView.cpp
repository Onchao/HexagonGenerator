#include "HexView.h"
#include "HexagonGenerator.h"

HexView::HexView(Hexagon hex, HexagonsGraph* graph, int shiftX, int shiftY, HexagonGenerator* hexagonGenerator) :
	hexagon(hex),
	hexagonsGraph(graph),
	hexagonGenerator(hexagonGenerator)
{
	std::vector<QPointF> polygon(hex.getCartesianVertices(80, shiftX, shiftY));

	setPolygon(QPolygonF(polygon.begin(), polygon.end()));
	//QPen pen(Qt::black);
	//pen.setWidthF(2);
	//setPen(pen);
	updateHexColor();
}

void HexView::updateHexColor()
{
	if (hexagonsGraph->contains(hexagon)) {
		setBrush(Qt::transparent);
	}
	else {
		setBrush(Qt::gray);
	}
}

void HexView::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	if (event->button() == Qt::LeftButton)
	{
		if (hexagonsGraph->contains(hexagon)) {
			setBrush(Qt::gray);
			hexagonsGraph->erase(hexagon);
			hexagonGenerator->updateHexagonsCount(-1);
		}
		else {
			setBrush(Qt::transparent);
			hexagonsGraph->add(hexagon);
			hexagonGenerator->updateHexagonsCount(1);
		}

		QGraphicsPolygonItem::mousePressEvent(event);
	}
}

void HexView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	painter->setRenderHint(QPainter::Antialiasing);
	QGraphicsPolygonItem::paint(painter, option, widget);
}