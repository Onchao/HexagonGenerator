#include "HexagonsScene.h"
#include "LocationLabel.h"
#include "ResourceManager.h"
#include "HexagonGenerator.h"

HexagonsScene::HexagonsScene(HexagonsGraph* graph, int width, int height, HexagonGenerator* hexagonGenerator, QObject* parent) :
	QGraphicsScene(nullptr),
	hexagonsGraph(graph),
	hexagonsView(),
	hexagonGenerator(hexagonGenerator),
	locationLabel(nullptr)
{
	setSceneRect(0, 0, width, height);

	desertItem = addPixmap(ResourceManager::getDesert(width));
	forestItem = addPixmap(ResourceManager::getForest(width));
	plainsItem = addPixmap(ResourceManager::getPlains(width));
	desertItem->hide();
	forestItem->hide();
	plainsItem->hide();

	float i = -2.5;
	for (int y = -6; y <= 6; y++, i-=0.5) {
		for (int x = std::floor(i); x < std::floor(i) + 11.5; x++) {
			//if (x != 0 || y != 0)  continue; // DEBUG
			HexView* hex = new HexView(Hexagon(x,y), hexagonsGraph, width / 2, height / 2, hexagonGenerator);
			hexagonsView.push_back(hex);
			addItem(hex);
		}
	}
}

HexagonsScene::~HexagonsScene()
{
	hexagonsView.clear();
}

void HexagonsScene::updateHexColors()
{
	for (auto hex : hexagonsView) {
		hex->updateHexColor();
	}
}

void HexagonsScene::updateTerrain(Terrain type)
{
	desertItem->hide();
	forestItem->hide();
	plainsItem->hide();
	switch (type) {
	case Terrain::desert:
		desertItem->show();
		return;
	case Terrain::forest:
		forestItem->show();
		return;
	case Terrain::plains:
		plainsItem->show();
		return;
	}
}

void HexagonsScene::setLocationLabel(LocationLabel* label)
{
	locationLabel = label;
}

void HexagonsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::RightButton) {
		isRightButtonPressed = true;
		locationLabel->setCenterPosition(event->scenePos());
	}
	else {
		QGraphicsScene::mousePressEvent(event);
	}
}

void HexagonsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (isRightButtonPressed) {
		locationLabel->setCenterPosition(event->scenePos());
	}
	else {
		QGraphicsScene::mouseMoveEvent(event);
	}
}

void HexagonsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::RightButton) {
		isRightButtonPressed = false;
	}
	else {
		QGraphicsScene::mouseReleaseEvent(event);
	}
}

void HexagonsScene::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	// TODO: maybe add scaling here? locationLabel->setSize(...);
	event->accept();
}
