#include "FileGenerator.h"

FileGenerator::FileGenerator(HexagonsGraph* graph, LocationLabel* label) :
	hexagonsGraph(graph),
	existingLabel(label)
{
	std::cout << "name, size, gold, knowledge, population, defence, terrain\n";
	QDir dir;
	dir.mkpath("generated_locations");
}

void FileGenerator::generateFiles(const QString& name, int hexHeightMm, Terrain terrainType)
{
	QDir dir;
	dir.mkpath("generated_locations\\" + name);
	generate2dPrint(name, hexHeightMm, terrainType);
	generate2dPrintSimple(name, hexHeightMm, terrainType);
	generate3dPrint(name, hexHeightMm, true);
	generateLabelCard(name);
	generateCsv(name, hexHeightMm, terrainType);
	
	std::string locationInfo = 
		name.toStdString() + ","
		+ std::to_string(hexagonsGraph->getHexagons().size()) + ","
		+ std::string(terrainToString(terrainType)) + ","
		+ existingLabel->printInfo()
		+ "\n";
	std::cout << locationInfo;
}

void FileGenerator::generate2dPrint(const QString& name, int hexHeightMm, Terrain terrainType)
{
	const int hexHeightPx = mmToPx(hexHeightMm);
	const int targetPixmapSize = hexHeightPx * 10;
	const float penWidth = hexHeightPx / 35.0f;
	const float totalOffset = -penWidth - mmToPx(offset3dPrint + 0.14);

	std::vector<QPointF> allVertices;
	std::vector<QPointF> hexCenters;
	std::unordered_map<QPointF, std::vector<QPointF*>, QPointFHash, QPointFEqual> duplicatesMap;
	std::vector<QPointF> hullCycle;
	prepare(allVertices, hexCenters, duplicatesMap, hullCycle, hexHeightPx, targetPixmapSize);

	std::vector<QPointF> hullCycleOffsetted = offsetVertices(hullCycle, totalOffset, hexHeightPx);

	QPolygonF polygon(hullCycleOffsetted.begin(), hullCycleOffsetted.end());
	QGraphicsPolygonItem* polygonItem = new QGraphicsPolygonItem(polygon);

	QBrush brush;
	switch (terrainType) {
	case Terrain::desert:
		brush.setTexture(ResourceManager::getDesert(targetPixmapSize));
		break;
	case Terrain::forest:
		brush.setTexture(ResourceManager::getForest(targetPixmapSize));
		break;
	case Terrain::plains:
		brush.setTexture(ResourceManager::getPlains(targetPixmapSize));
		break;
	}

	polygonItem->setBrush(brush);
	QPen pen(Qt::black);
	pen.setWidthF(penWidth);
	pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
	polygonItem->setPen(pen);

	QGraphicsScene scene;
	scene.addItem(polygonItem);
	LocationLabel newLabel(&scene, existingLabel, targetPixmapSize, false);

	QRectF sceneRect = scene.sceneRect();
	QImage image(sceneRect.size().toSize(), QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	QPainter painter(&image);
	painter.setRenderHint(QPainter::Antialiasing);

	scene.render(&painter);
	painter.end();

	image.setDotsPerMeterX(dpi * 39.3701);
	image.setDotsPerMeterY(dpi * 39.3701);
	image.save("generated_locations\\" + name + "\\" + name + ".png");
}

void FileGenerator::generate2dPrintSimple(const QString& name, int hexHeightMm, Terrain terrainType)
{
	const int hexHeightPx = mmToPx(hexHeightMm);
	const int targetPixmapSize = hexHeightPx * 10;
	const float penWidth = hexHeightPx / 35.0f;

	const float additionalOffset = - mmToPx(2);
	const float teethDepth = 0.1 * hexHeightMm / 2 * (2 / std::sqrt(3));
	const float totalOffset = -penWidth - mmToPx(offset3dPrint + teethDepth + 0.7);

	std::vector<QPointF> allVertices;
	std::vector<QPointF> hexCenters;
	std::unordered_map<QPointF, std::vector<QPointF*>, QPointFHash, QPointFEqual> duplicatesMap;
	std::vector<QPointF> hullCycle;
	prepare(allVertices, hexCenters, duplicatesMap, hullCycle, hexHeightPx, targetPixmapSize);

	// remove non hexagon corners vertices from the cycle
	std::vector<QPointF> hullCycleSimple;
	for (int i = 0; i < hullCycle.size(); ++i) {
		if (i % 7 == 0) {
			hullCycleSimple.push_back(hullCycle[i]);
		}
	}

	std::vector<QPointF> hullCycleOffsetted = offsetVertices(hullCycleSimple, totalOffset, hexHeightPx);

	std::vector<QPointF> smoothedCycle;
	for (int i = 0; i < hullCycleOffsetted.size(); ++i) {
		const QPointF& prev = hullCycleOffsetted[(i-1 + hullCycleOffsetted.size())% hullCycleOffsetted.size()];
		const QPointF& curr = hullCycleOffsetted[i];
		const QPointF& next = hullCycleOffsetted[(i+1) % hullCycleOffsetted.size()];

		if (crossProduct(prev, curr, next) < 0) { // if is concave with its neighbors
			smoothedCycle.push_back(curr - (curr - prev) * 0.27);
			smoothedCycle.push_back(curr + (next - curr) * 0.27);
		}
		else { // otherwise keep original
			smoothedCycle.push_back(curr); 
		}
	}

	QPolygonF polygon(smoothedCycle.begin(), smoothedCycle.end());
	QGraphicsPolygonItem* polygonItem = new QGraphicsPolygonItem(polygon);

	QBrush brush;
	switch (terrainType) {
	case Terrain::desert:
		brush.setTexture(ResourceManager::getDesert(targetPixmapSize));
		break;
	case Terrain::forest:
		brush.setTexture(ResourceManager::getForest(targetPixmapSize));
		break;
	case Terrain::plains:
		brush.setTexture(ResourceManager::getPlains(targetPixmapSize));
		break;
	}

	polygonItem->setBrush(brush);
	QPen pen(Qt::black);
	pen.setWidthF(penWidth);
	pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
	polygonItem->setPen(pen);

	QGraphicsScene scene;
	scene.addItem(polygonItem);
	LocationLabel newLabel(&scene, existingLabel, targetPixmapSize, true);

	QRectF sceneRect = scene.sceneRect();
	QImage image(sceneRect.size().toSize(), QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	QPainter painter(&image);
	painter.setRenderHint(QPainter::Antialiasing);

	scene.render(&painter);
	painter.end();

	image.setDotsPerMeterX(dpi * 39.3701);
	image.setDotsPerMeterY(dpi * 39.3701);
	image.save("generated_locations\\" + name + "\\" + name + " simple.png");
}

void FileGenerator::generateLabelCard(const QString& name)
{
	int widthMm = 120;
	LocationCard locationCard(existingLabel, mmToPx(widthMm));
	
	QPixmap pixmap(locationCard.width(), locationCard.height());
	pixmap.fill(Qt::transparent);
	locationCard.render(&pixmap);
	QImage image = pixmap.toImage();

	image.setDotsPerMeterX(dpi * 39.3701);
	image.setDotsPerMeterY(dpi * 39.3701);
	image.save("generated_locations\\" + name + "\\" + name + " card.png");
}

void FileGenerator::generate3dPrint(const QString& name, int hexHeightMm, bool flipY)
{
	const int tileHeight = 2; // [mm]
	const int targetPixmapSize = 10 * hexHeightMm; // mm = px

	std::vector<QPointF> allVertices;
	std::vector<QPointF> hexCenters;
	std::unordered_map<QPointF, std::vector<QPointF*>, QPointFHash, QPointFEqual> duplicatesMap;
	std::vector<QPointF> hullCycle;
	prepare(allVertices, hexCenters, duplicatesMap, hullCycle, hexHeightMm, targetPixmapSize);
	std::vector<QPointF> hullCycleOffsetted = offsetVertices(hullCycle, offset3dPrint, hexHeightMm);

	for (int i = 0; i < hullCycle.size(); ++i) {
		for (QPointF* v : duplicatesMap[hullCycle[i]]) {
			*v = hullCycleOffsetted[i];
		}
	}

	if (flipY) {
		for (QPointF& v : allVertices) {
			v.setY(-v.y());
		}
		for (QPointF& v : hexCenters) {
			v.setY(-v.y());
		}
		for (QPointF& v : hullCycleOffsetted) {
			v.setY(-v.y());
		}
	}

	std::vector<Triangle> triangles;

	// sides:
	for (int i = 0; i < hullCycleOffsetted.size(); ++i) {
		QPointF& a = hullCycleOffsetted[i];
		QPointF& b = hullCycleOffsetted[(i + 1) % hullCycleOffsetted.size()];

		QVector3D botA(a.x(), a.y(), 0);
		QVector3D botB(b.x(), b.y(), 0);
		QVector3D topA(a.x(), a.y(), tileHeight);
		QVector3D topB(b.x(), b.y(), tileHeight);

		flipY ? triangles.emplace_back(botA, topB, botB) : triangles.emplace_back(botA, botB, topB);
		flipY ? triangles.emplace_back(botA, topA, topB) : triangles.emplace_back(botA, topB, topA);
	}

	// bottom + top:
	for (int i = 0; i < hexCenters.size(); ++i) {
		for (int height = 0; height <= tileHeight; height += tileHeight) { // 0 for the bottom and 'tileHeight' for the top
			/*
			  C
			 / \
			A---B

			flip=1 height=0 -> ABC (counterclockwise)
			flip=0 height=0 -> ACB (clockwise)
			flip=1 height=1 -> ACB (clockwise)
			flip=0 height=1 -> ABC (counterclockwise)

			flip XOR height
			*/
			bool flipNormals = flipY ^ (bool)height;

			constexpr int n = 6 * 7; // number of vertices in a tile hex. there are 6 symmetric sides and each has 7 vertices
			QVector3D V[n]; // single hex tile description
			for (int v = 0; v < n; ++v) {
				V[v].setX(allVertices[i * n + v].x());
				V[v].setY(allVertices[i * n + v].y());
				V[v].setZ(height);
			}
			QVector3D center(hexCenters[i].x(), hexCenters[i].y(), height);

			for (int j = 0; j < n; j += 7) { // for each symmetric side seperately
				QVector3D v40 = V[(j - 2 + n) % n];
				QVector3D v41 = V[(j - 1 + n) % n];
				QVector3D v0 = V[(j + 0) % n];
				QVector3D v1 = V[(j + 1) % n];
				QVector3D v2 = V[(j + 2) % n];
				QVector3D v3 = V[(j + 3) % n];
				QVector3D v4 = V[(j + 4) % n];
				QVector3D v5 = V[(j + 5) % n];

				if (flipNormals) {
					triangles.emplace_back(v40, v41, v0);
					triangles.emplace_back(v40, v0, v1);
					triangles.emplace_back(v2, v3, v4);
					triangles.emplace_back(v1, v2, v4);
					triangles.emplace_back(v40, v1, center);
					triangles.emplace_back(v1, v4, center);
					triangles.emplace_back(v4, v5, center);
				}
				else {
					triangles.emplace_back(v40, v0, v41);
					triangles.emplace_back(v40, v1, v0);
					triangles.emplace_back(v2, v4, v3);
					triangles.emplace_back(v1, v4, v2);
					triangles.emplace_back(v40, center, v1);
					triangles.emplace_back(v1, center, v4);
					triangles.emplace_back(v4, center, v5);
				}
			}
		}
	}

	writeSTL(name, triangles);
}

void FileGenerator::generateCsv(const QString& name, int hexHeightMm, Terrain terrainType)
{
	//TODO: there should be struct for that and reading
	std::string locationInfo =
		name.toStdString() + ","
		+ std::to_string(hexagonsGraph->getHexagons().size()) + ","
		+ std::string(terrainToString(terrainType)) + ","
		+ existingLabel->printInfo();

	std::string transform = existingLabel->printTransform();

	std::string graphInfo;
	for (auto [x, y] : hexagonsGraph->getGraph()) {
		graphInfo.append("," + std::to_string(x) + "," + std::to_string(y));
	}

	std::string data = locationInfo + "," + transform + graphInfo;

	QFile file("generated_locations\\" + name + "\\" + name + ".csv");

	if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream out(&file);
		out << QString::fromStdString(data);
		file.close();
	}
}

float FileGenerator::mmToPx(const float mmDist)
{
	return mmDist * dpi / 25.4f;
}

void FileGenerator::prepare(
		std::vector<QPointF>& allVertices,
		std::vector<QPointF>& hexCenters,
		std::unordered_map<QPointF, std::vector<QPointF*>, QPointFHash, QPointFEqual>& duplicatesMap, // maps vector positions to duplicates from allVertices
		std::vector<QPointF>& hullCycle, // starts with the lowest left vertex
		int hexHeightPx,
		int targetPixmapSize)
{
	int hexDescriptionLength = hexagonsGraph->getHexagons().begin()
		->getCartesianVertices(hexHeightPx, targetPixmapSize / 2, targetPixmapSize / 2).size();
	allVertices.reserve(hexagonsGraph->size() * hexDescriptionLength);

	for (const auto& hex : hexagonsGraph->getHexagons()) {
		std::vector<QPointF> hexagonVertices
			= hex.getCartesianVertices(hexHeightPx, targetPixmapSize / 2, targetPixmapSize / 2);
		allVertices.insert(allVertices.end(), hexagonVertices.begin(), hexagonVertices.end());
		hexCenters.push_back(hex.getCartesianCenter(hexHeightPx, targetPixmapSize / 2, targetPixmapSize / 2));
	}

	for (int i = 0; i < allVertices.size(); ++i) {
		duplicatesMap[allVertices[i]].push_back(&allVertices[i]);
	}

	std::unordered_set<std::pair<QPointF, QPointF>, EdgeHash, EdgeEqual> hullEdges;
	hullEdges.reserve(allVertices.size());
	for (int i = 0; i < allVertices.size() / hexDescriptionLength; ++i) {
		for (int j = 0; j < hexDescriptionLength; ++j) {
			QPointF u = allVertices[i * hexDescriptionLength + j];
			QPointF v = allVertices[i * hexDescriptionLength + (j + 1) % hexDescriptionLength];

			std::pair<QPointF, QPointF> edge{ u,v };
			if (hullEdges.contains(edge)) {
				hullEdges.erase(edge);
			}
			else
			{
				hullEdges.insert(edge);
			}
		}
	}

	std::unordered_map<QPointF, QPointF, QPointFHash, QPointFEqual> adjacencyList;
	adjacencyList.reserve(hullEdges.size());
	for (const auto& [u, v] : hullEdges) {
		adjacencyList[u] = v;
	}

	hullCycle.reserve(adjacencyList.size());
	QPointF lowest = allVertices[0];
	for (int i = 1; i < allVertices.size(); ++i) {
		const QPointF& v = allVertices[i];
		if (v.y() < lowest.y() || (v.y() == lowest.y() && v.x() < lowest.x())) {
			lowest = v;
		}
	}
	QPointF next = lowest;
	do {
		hullCycle.push_back(next);
		next = adjacencyList[hullCycle.back()];
	} while (next != lowest);
}

std::vector<QPointF> FileGenerator::offsetVertices(
		std::vector<QPointF>& hullCycle, 
		float dist, 
		float hexHeightPx)
{
	//dist=0;
	std::vector<QPointF> hullCycleOffsetted;
	hullCycleOffsetted.reserve(hullCycle.size());

	for (int i = 0; i < hullCycle.size(); ++i) {
		/*        
			E
		A   |   C
		  \ | /
			B
			|
			F    
		*/
		const QPointF A = hullCycle[(i - 1 + hullCycle.size()) % hullCycle.size()];
		const QPointF B = hullCycle[i]; // we are offseting this point
		const QPointF C = hullCycle[(i + 1) % hullCycle.size()];

		const QPointF BA = A - B;
		const QPointF BC = C - B;
		const QPointF uBA = unitVector(BA);
		const QPointF uBC = unitVector(BC);

		const QPointF BD = uBA + uBC; // both points E and F will be on this vector
		const QPointF uBD = unitVector(BD);
		
		// we are shifting B vertex so the distance must be scaled based on angle
		// shift = dist / sin(ABD)
		// sin(ABD) = |BA x BD| / (|BA|*|BD|)
		// shift = dist * |BA|*|BD| / |BA x BD|
		// after substituting unit vector lengths: shift = dist / |uBA x uBD|

		const float shift = dist / abs(crossProduct(uBA, QPointF(0, 0), uBD));

		const QPointF shiftVec = uBD * shift;
		const QPointF E = B + shiftVec;
		const QPointF F = B - shiftVec;

		// now add vertex E or F, which with last(), does not cross AB
		if (i == 0) {
			hullCycleOffsetted.push_back(F); // positive offset produces bigger shape
		}
		else {
			if (sameSide(A, B, hullCycleOffsetted.back(), E)) {
				hullCycleOffsetted.push_back(E);
			}
			else {
				hullCycleOffsetted.push_back(F);
			}
		}        
	}
	return hullCycleOffsetted;
}

QPointF FileGenerator::unitVector(const QPointF& point) const
{
	qreal length = qSqrt(point.x() * point.x() + point.y() * point.y());
	return (length == 0) ? QPointF(0, 0) : QPointF(point.x() / length, point.y() / length);
};

float FileGenerator::crossProduct(const QPointF& A, const QPointF& B, const QPointF& C) const
{
	return (B.x() - A.x()) * (C.y() - A.y()) - (B.y() - A.y()) * (C.x() - A.x());
}

bool FileGenerator::sameSide(const QPointF& A, const QPointF& B, const QPointF& C, const QPointF& D) const
{
	double crossC = crossProduct(A, B, C);
	double crossD = crossProduct(A, B, D);
	return crossC * crossD > 0;
}

void FileGenerator::writeSTL(const QString& name, const std::vector<Triangle>& triangles)
{
	QString filename = "generated_locations\\" + name + "\\" + name + ".stl";
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qCritical() << "Failed to open file:" << filename;
		return;
	}

	QString betterName(name);
	betterName.replace(" ", "_");

	QTextStream stream(&file);
	stream << "solid" << betterName << "\n";

	for (const auto& triangle : triangles) {
		stream << "  facet normal " << triangle.normal.x() << " " << triangle.normal.y() << " " << triangle.normal.z() << "\n";
		stream << "    outer loop\n";
		stream << "      vertex " << triangle.v1.x() << " " << triangle.v1.y() << " " << triangle.v1.z() << "\n";
		stream << "      vertex " << triangle.v2.x() << " " << triangle.v2.y() << " " << triangle.v2.z() << "\n";
		stream << "      vertex " << triangle.v3.x() << " " << triangle.v3.y() << " " << triangle.v3.z() << "\n";
		stream << "    endloop\n";
		stream << "  endfacet\n";
	}

	stream << "endsolid " << betterName << "\n";
	file.close();
}