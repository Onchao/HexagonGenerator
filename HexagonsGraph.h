#pragma once 
#include <map>
#include <set>
#include <vector>
#include <random>
#include <iostream>
#include <QPointF>
#include <QPolygon>
#include <array>

class Hexagon
{
public:
	int x;
	int y;

	std::vector<QPointF> getCartesianVertices(float hexSize, float shiftX, float shiftY) const;
	QPointF getCartesianCenter(float hexSize, float shiftX, float shiftY) const;
	std::vector<Hexagon> getNeighbours() const;

	auto operator<=>(const Hexagon& other) const = default;

	int addTeethToEdge(std::vector<QPointF>& toothVertices, QPointF u, QPointF v) const;
	int addTeethToEdgeSimple(std::vector<QPointF>& toothVertices, QPointF u, QPointF v) const; // won't work, no code for it in FileGenerator
};


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
	void print();
private:
	std::set<Hexagon> getFreeAdjacent();

	std::set<Hexagon> hexagons;

	std::random_device rd;
	std::mt19937 gen;
};
