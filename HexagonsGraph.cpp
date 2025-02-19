#include "HexagonsGraph.h"


HexagonsGraph::HexagonsGraph(int n) : gen(rd())
{
	randomizeGraph(n);
}

void HexagonsGraph::randomizeGraph(int n)
{
	hexagons.clear();
	if (n == 0) {
		return;
	}
	hexagons.insert(Hexagon{ 0,0 });

	for (int i = 2; i <= n; ++i) {
		std::set<Hexagon> freeAdjacent = getFreeAdjacent();
		std::uniform_int_distribution<> randomHex(0, freeAdjacent.size() - 1);
		std::set<Hexagon>::iterator it = freeAdjacent.begin();
		std::advance(it, randomHex(gen));
		hexagons.insert(*it);
	}
}

bool HexagonsGraph::contains(const Hexagon& other) const
{
	return hexagons.contains(other);
}

void HexagonsGraph::add(Hexagon newHex)
{
	hexagons.insert(std::move(newHex));
}

void HexagonsGraph::erase(const Hexagon& removeHex)
{
	hexagons.erase(removeHex);
}

const std::set<Hexagon>& HexagonsGraph::getHexagons() const
{ 
	return hexagons;
}

size_t HexagonsGraph::size() const
{
	return hexagons.size();
}

std::vector<std::pair<int, int>> HexagonsGraph::getGraph()
{
	std::vector<std::pair<int, int>> result;
	result.reserve(hexagons.size());
	for (const Hexagon& h : hexagons) {
		result.emplace_back(h.x, h.y);
	}
	return result;
}

void HexagonsGraph::setGraph(std::vector<std::pair<int, int>>& newGraph)
{
	hexagons.clear();
	for (auto [x, y] : newGraph) {
		hexagons.insert(Hexagon(x, y));
	}
}

void HexagonsGraph::print()
{
	for (const auto& hex : hexagons) {
		std::cout << " (" << hex.x << " " << hex.y << ")   ";
	}
	std::cout << std::endl;
}

std::set<Hexagon> HexagonsGraph::getFreeAdjacent()
{
	std::set<Hexagon> result;

	for (const auto& hex : hexagons) {
		std::vector<Hexagon> neigbhours = hex.getNeighbours();
		result.insert(neigbhours.begin(), neigbhours.end());
	}

	for (const auto& hex : hexagons) {
		result.erase(hex);
	}

	return result;
}

