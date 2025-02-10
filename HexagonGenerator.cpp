#include "HexagonGenerator.h"

HexagonGenerator::HexagonGenerator(QWidget* parent) :
	QMainWindow(parent),
	hexagonsGraph(0)
{
	ui.setupUi(this);
	this->installEventFilter(this);

	dial = new CustomDial(this);
	
	connect(ui.randomize, &QPushButton::pressed, this, &HexagonGenerator::randomizeGraph);
	connect(ui.hexagonsCount, &QSpinBox::valueChanged, this, &HexagonGenerator::randomizeGraph);

	connect(ui.desertButton, &QRadioButton::pressed, this, [this]() {this->updateTerrain(Terrain::desert); });
	connect(ui.forestButton, &QRadioButton::pressed, this, [this]() {this->updateTerrain(Terrain::forest); });
	connect(ui.plainsButton, &QRadioButton::pressed, this, [this]() {this->updateTerrain(Terrain::plains); });
	
	connect(ui.locationName, &QLineEdit::textChanged, this, &HexagonGenerator::updateLabelText);
	connect(ui.goldCount, &QSpinBox::valueChanged, this, &HexagonGenerator::updateLabelText);
	connect(ui.knowledgeCount, &QSpinBox::valueChanged, this, &HexagonGenerator::updateLabelText);
	connect(ui.populationCount, &QSpinBox::valueChanged, this, &HexagonGenerator::updateLabelText);
	connect(ui.defenceCount, &QSpinBox::valueChanged, this, &HexagonGenerator::updateLabelText);

	scene = new HexagonsScene(&hexagonsGraph, ui.graphicsView->width(), ui.graphicsView->height(), this);

	locationLabel = new LocationLabel(scene);
	QObject::connect(dial, &CustomDial::valueChanged, this, [this]() {locationLabel->setRotation(dial->value()); });
	connect(ui.sizeSlider, &QSlider::valueChanged, this, [this]() {locationLabel->setSize(ui.sizeSlider->value()); });
	locationLabel->setSize(ui.sizeSlider->value());
	locationLabel->setCenterPosition({ 800 / 2, 800 / 2 });
	connect(ui.labelCenter, &QCheckBox::checkStateChanged, this, [this]() {locationLabel->showCenterDot(ui.labelCenter->isChecked()); });
	connect(ui.showLabel, &QCheckBox::checkStateChanged, this, [this]() {locationLabel->showLabel(ui.showLabel->isChecked()); });
	scene->setLocationLabel(locationLabel);

	ui.graphicsView->setScene(scene);
	ui.graphicsView->show();

	ui.desertButton->animateClick();
	randomizeGraph();
	updateLabelText();

	fileGenerator = new FileGenerator(&hexagonsGraph, locationLabel);
	connect(ui.generateFiles, &QPushButton::pressed, this, &HexagonGenerator::startFileGeneration);
}

HexagonGenerator::~HexagonGenerator()
{
	delete dial;
	delete scene;
	delete fileGenerator;
}

void HexagonGenerator::randomizeGraph()
{
	hexagonsGraph.randomizeGraph(ui.hexagonsCount->value());
	scene->updateHexColors();
}

void HexagonGenerator::updateLabelText()
{
	locationLabel->updateText(
		ui.locationName->text(),
		ui.goldCount->value(),
		ui.knowledgeCount->value(),
		ui.populationCount->value(),
		ui.defenceCount->value());
}

void HexagonGenerator::updateTerrain(Terrain terrain)
{
	scene->updateTerrain(terrain);
}

void HexagonGenerator::startFileGeneration()
{
	Terrain currentTerrain;
	if (ui.desertButton->isChecked()) {
		currentTerrain = Terrain::desert;
	}
	else if (ui.forestButton->isChecked()) {
		currentTerrain = Terrain::forest;
	}
	else if (ui.plainsButton->isChecked()) {
		currentTerrain = Terrain::plains;
	}

	fileGenerator->generateFiles(ui.locationName->text(), ui.hexHeight->value(), currentTerrain);
}

void HexagonGenerator::updateHexagonsCount(int delta) {
	ui.hexagonsCount->blockSignals(true);
	ui.hexagonsCount->setValue(ui.hexagonsCount->value() + delta);
	ui.hexagonsCount->blockSignals(false);
}