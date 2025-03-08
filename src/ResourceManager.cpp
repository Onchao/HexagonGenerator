#include "ResourceManager.h"

std::unique_ptr<QPixmap> ResourceManager::desert = nullptr;
std::unique_ptr<QPixmap> ResourceManager::forest = nullptr;
std::unique_ptr<QPixmap> ResourceManager::plains = nullptr;
std::unique_ptr<QPixmap> ResourceManager::label = nullptr;
std::unique_ptr<QPixmap> ResourceManager::card = nullptr;

void ResourceManager::initializePixmaps()
{
	desert = std::make_unique<QPixmap>(":/HexagonGenerator/graphics/desert.png");
	forest = std::make_unique<QPixmap>(":/HexagonGenerator/graphics/forest.png");
	plains = std::make_unique<QPixmap>(":/HexagonGenerator/graphics/plains.png");
	label = std::make_unique<QPixmap>(":/HexagonGenerator/graphics/label.png");
	card = std::make_unique<QPixmap>(":/HexagonGenerator/graphics/card.png");
}

const QPixmap ResourceManager::getDesert(int targetSize)
{
	return desert->scaled(
		{ targetSize, targetSize },
		Qt::KeepAspectRatio,
		Qt::SmoothTransformation
	);
}

const QPixmap ResourceManager::getForest(int targetSize)
{
	return forest->scaled(
		{ targetSize, targetSize },
		Qt::KeepAspectRatio,
		Qt::SmoothTransformation
	);
}

const QPixmap ResourceManager::getPlains(int targetSize)
{
	return plains->scaled(
		{ targetSize, targetSize },
		Qt::KeepAspectRatio,
		Qt::SmoothTransformation
	);
}

const QPixmap ResourceManager::getLabel(int targetWidth, int targetHeight)
{
	return label->scaled(
		{ targetWidth, targetHeight },
		Qt::KeepAspectRatio,
		Qt::SmoothTransformation
	);
}

const QPixmap ResourceManager::getCard(int targetWidth, int targetHeight)
{
	return card->scaled(
		{ targetWidth, std::numeric_limits<int>::max() }, // Qt::KeepAspectRatio will ignore height setting
		Qt::KeepAspectRatio,
		Qt::SmoothTransformation
	);
}
