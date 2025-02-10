#include "HexagonsGraph.h"


std::vector<QPointF> Hexagon::getCartesianVertices(float hexSize, float shiftX, float shiftY) const
{
	std::array<QPointF, 6> vertices;
	// comments on the default x,y coordinate system (no flipping y)
	vertices[0] = { (x)     * 1.0f + (y)     * 0.5f + 0.5f, (y)     * 0.5f * sqrt(3) - 0.5f / sqrt(3) }; // bot right corner
	vertices[1] = { (x)     * 1.0f + (y)     * 0.5f + 0.5f, (y)     * 0.5f * sqrt(3) + 0.5f / sqrt(3) }; // top right  corner
	vertices[2] = { (x - 1) * 1.0f + (y + 1) * 0.5f + 0.5f, (y + 1) * 0.5f * sqrt(3) - 0.5f / sqrt(3) }; // top corner
	vertices[3] = { (x - 1) * 1.0f + (y)     * 0.5f + 0.5f, (y)     * 0.5f * sqrt(3) + 0.5f / sqrt(3) }; // top left corner
	vertices[4] = { (x - 1) * 1.0f + (y)     * 0.5f + 0.5f, (y)     * 0.5f * sqrt(3) - 0.5f / sqrt(3) }; // bot left corner
	vertices[5] = { (x)     * 1.0f + (y - 1) * 0.5f + 0.5f, (y - 1) * 0.5f * sqrt(3) + 0.5f / sqrt(3) }; // bot corner

	std::vector<QPointF> toothVertices;
	int toothVericesCount;
	for (int i = 0; i < 6; ++i) {
		toothVericesCount = addTeethToEdge(toothVertices, vertices[i], vertices[(i + 1) % 6]);
	}

	// mix vertices so they are in the correct order
	std::vector<QPointF> result;
	result.reserve(6 + 6 * toothVericesCount);
	for (int i = 0; i < 6; ++i) {
		result.push_back(vertices[i]);
		for (int j = 0; j < toothVericesCount; j++) {
			result.push_back(toothVertices[i * toothVericesCount + j]);
		}
	}

	// scale to size
	for (QPointF& point : result) {
		point.setX(point.x() * hexSize + shiftX);
		point.setY(point.y() * hexSize + shiftY);
	}

	return result;
}

QPointF Hexagon::getCartesianCenter(float hexSize, float shiftX, float shiftY) const
{ 
	QPointF point{ (x) * 1.0f + (y) * 0.5f, (y) * 0.5f * sqrt(3) };
	point.setX(point.x() * hexSize + shiftX);
	point.setY(point.y() * hexSize + shiftY);
	return point;
}


std::vector<Hexagon> Hexagon::getNeighbours() const
{
	std::vector<Hexagon> result;
	result.push_back(Hexagon{ x + 1, y });
	result.push_back(Hexagon{ x + 1, y - 1 });
	result.push_back(Hexagon{ x, y + 1 });
	result.push_back(Hexagon{ x, y - 1});
	result.push_back(Hexagon{ x - 1, y + 1});
	result.push_back(Hexagon{ x - 1, y});

	return result;
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

int Hexagon::addTeethToEdge(std::vector<QPointF>& toothVertices, QPointF u, QPointF v) const
{
	bool swapped = false;
	// ensure that teeth are always selected in the same order
	// to avoid floating point rounding errors
	if (u.y() > v.y()) {
		std::swap(u, v);
		swapped = true;
	}
	/*
		B    E----F
		/ |    |    |
	A   +    +    +   H
		|	 |    | /
		C----D    G
	*/
	QPointF A{ 0,0 };
	QPointF H{ v.x() - u.x(), v.y() - u.y() };

	float distA_BC = 0.235; // |A(BC)|=|(FG)H|
	float distA_DE = 0.5; // center
	float depth = 0.1;

	QPointF BC = H * distA_BC;
	QPointF ED = H * distA_DE;
	QPointF FG = H - H * distA_BC;

	QPointF B = BC + QPointF(-H.y(), H.x()) * depth;
	QPointF E = ED + QPointF(-H.y(), H.x()) * depth;
	QPointF F = FG + QPointF(-H.y(), H.x()) * depth;

	QPointF C = BC + QPointF(H.y(), -H.x()) * depth;
	QPointF D = ED + QPointF(H.y(), -H.x()) * depth;
	QPointF G = FG + QPointF(H.y(), -H.x()) * depth;

	if (!swapped) {
		toothVertices.push_back(B + u);
		toothVertices.push_back(C + u);
		toothVertices.push_back(D + u);
		toothVertices.push_back(E + u);
		toothVertices.push_back(F + u);
		toothVertices.push_back(G + u);
	}
	else {
		toothVertices.push_back(G + u);
		toothVertices.push_back(F + u);
		toothVertices.push_back(E + u);
		toothVertices.push_back(D + u);
		toothVertices.push_back(C + u);
		toothVertices.push_back(B + u);
	}

	return 6;
}

int Hexagon::addTeethToEdgeSimple(std::vector<QPointF>& toothVertices, QPointF u, QPointF v) const
{
	/*
		 C--D
		 |  |
	A----B  E----F  I----J
				 |  |
				 G--H
	*/

	// add swap if using this code
	QPointF A{ 0,0 };
	QPointF J{ v.x() - u.x(), v.y() - u.y() };


	float distAB = 0.25; // |AB|=|IJ|
	float distBE = 0.25; // |BE|=|FI|
	float distBC = 0.1;

	QPointF B = J * distAB;
	QPointF I = J - J * distAB;
	QPointF E = J * (distAB + distBE);
	QPointF F = J - J * (distAB + distBE);

	QPointF C = B + QPointF(-J.y(), J.x()) * distBC;
	QPointF D = E + QPointF(-J.y(), J.x()) * distBC;
	QPointF G = F + QPointF(J.y(), -J.x()) * distBC;
	QPointF H = I + QPointF(J.y(), -J.x()) * distBC;

	toothVertices.push_back(B + u);
	toothVertices.push_back(C + u);
	toothVertices.push_back(D + u);
	toothVertices.push_back(E + u);
	toothVertices.push_back(F + u);
	toothVertices.push_back(G + u);
	toothVertices.push_back(H + u);
	toothVertices.push_back(I + u);

	return 8;
};

///////////////////////////////////////////////////////////////////////////////

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

void HexagonsGraph::print()
{
	for (const auto& hex : hexagons) {
		std::cout << " (" << hex.x << " " << hex.y << ")   ";
	}
	std::cout << std::endl;
}


