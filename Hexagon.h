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
