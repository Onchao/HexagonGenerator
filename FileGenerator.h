#pragma once

#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <QPolygonF>
#include <QBrush>
#include <QPen>
#include <set>
#include <vector>
#include <LocationLabel.h>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <QDir>

#include "ResourceManager.h"
#include "HexagonsGraph.h"
#include "LocationCard.h"

struct Triangle {
	QVector3D normal;
	QVector3D v1, v2, v3;

	Triangle(const QVector3D& vertex1, const QVector3D& vertex2, const QVector3D& vertex3)
		: v1(vertex1), v2(vertex2), v3(vertex3) {
		QVector3D u = v2 - v1;
		QVector3D v = v3 - v1;
		normal = QVector3D::crossProduct(u, v).normalized();
	}
};

// custom unorderd map
struct QPointFHash {
	std::size_t operator()(const QPointF& point) const {
		std::size_t h1 = std::hash<double>{}(point.x());
		std::size_t h2 = std::hash<double>{}(point.y());
		return h1 ^ (h2 << 1);
	}
};
struct QPointFEqual {
	bool operator()(const QPointF& lhs, const QPointF& rhs) const {
		return lhs == rhs;
	}
};

// custom unorderd set for edges (u,v)=(v,u)
inline std::size_t hashCombine(std::size_t lhs, std::size_t rhs) {
	return lhs ^ (rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2));
}
struct EdgeHash {
	std::size_t operator()(const std::pair<QPointF, QPointF>& edge) const noexcept {
		std::size_t h1 = std::hash<double>{}(edge.first.x()) ^ std::hash<double>{}(edge.first.y());
		std::size_t h2 = std::hash<double>{}(edge.second.x()) ^ std::hash<double>{}(edge.second.y());

		// ensure (u, v) and (v, u) hash identically by always combining in the same order
		return hashCombine(std::min(h1, h2), std::max(h1, h2));
	}
};
struct EdgeEqual {
	bool operator()(const std::pair<QPointF, QPointF>& lhs, const std::pair<QPointF, QPointF>& rhs) const noexcept {
		return (lhs.first == rhs.first && lhs.second == rhs.second) ||
			(lhs.first == rhs.second && lhs.second == rhs.first);
	}
};


class FileGenerator
{
public:
	FileGenerator(HexagonsGraph* graph, LocationLabel* label);
	void generateFiles(const QString& name, int hexHeightMm, Terrain terrainType);

private:
	void generate2dPrint(const QString& name, int hexHeightMm, Terrain terrainType);
	void generate2dPrintSimple(const QString& name, int hexHeightMm, Terrain terrainType);
	void generateLabelCard(const QString& name);
	void generate3dPrint(const QString& name, int hexHeightMm, bool flipY);
	void generateCsv(const QString& name, int hexHeightMm, Terrain terrainType);
	float mmToPx(const float mmDist);
	void prepare(
		std::vector<QPointF>& allVertices,
		std::vector<QPointF>& hexCenters,
		std::unordered_map<QPointF, std::vector<QPointF*>, QPointFHash, QPointFEqual>& duplicatesMap,
		std::vector<QPointF>& hullCycle,
		int hexHeightPx,
		int targetPixmapSize);
	std::vector<QPointF> offsetVertices(
		std::vector<QPointF>& hullCycle,
		float dist,
		float hexHeightPx);
	QPointF unitVector(const QPointF& point) const;
	float crossProduct(const QPointF& A, const QPointF& B, const QPointF& C) const;
	bool sameSide(const QPointF& A, const QPointF& B, const QPointF& C, const QPointF& D) const; // returns true <=> C and D are on the same side of AB
	void writeSTL(const QString& name, const std::vector<Triangle>& triangles);

	HexagonsGraph* hexagonsGraph;
	LocationLabel* existingLabel;
	const int dpi = 300;
	const float offset3dPrint = -0.07; // [mm]

};

