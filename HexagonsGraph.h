#pragma once 
#include <map>
#include <set>
#include <vector>
#include <random>
#include <iostream>
#include <QPointF>
#include <QPolygon>
#include <array>

#include "Hexagon.h"

class HexagonsGraph
{
public:
	HexagonsGraph(int n);
	void randomizeGraph(int n);
	bool contains(const Hexagon& other) const;
	void add(Hexagon newHex);
	void erase(const Hexagon& removeHex);
	const std::set<Hexagon>& getHexagons() const;
	size_t size() const;
	std::vector<std::pair<int, int>> getGraph();
	void setGraph(std::vector<std::pair<int, int>>& newGraph);
	void print();
private:
	std::set<Hexagon> getFreeAdjacent();

	std::set<Hexagon> hexagons;

	std::random_device rd;
	std::mt19937 gen;
};
