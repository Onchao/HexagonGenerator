#pragma once

#include <QPixmap>
#include <iostream>

class ResourceManager
{
public:
	static void initializePixmaps();
	static const QPixmap getDesert(int targetSize);
	static const QPixmap getForest(int targetSize);
	static const QPixmap getPlains(int targetSize);
	static const QPixmap getLabel(int targetWidth, int targetHeight);
	static const QPixmap getCard(int targetWidth, int targetHeight);

private:
	static std::unique_ptr<QPixmap> desert;
	static std::unique_ptr<QPixmap> forest;
	static std::unique_ptr<QPixmap> plains;
	static std::unique_ptr<QPixmap> label;
	static std::unique_ptr<QPixmap> card;
};

enum class Terrain
{
	desert,
	forest,
	plains,
};

constexpr std::string_view terrainToString(Terrain type)
{
	switch (type)
	{
	case Terrain::desert: return "desert";
	case Terrain::forest: return "forest";
	case Terrain::plains: return "plains";
	}
}

constexpr Terrain stringToTerrain(std::string_view str)
{
	if (str == "desert") return Terrain::desert;
	if (str == "forest") return Terrain::forest;
	if (str == "plains") return Terrain::plains;
}
