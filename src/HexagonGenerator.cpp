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

	connect(ui.desertButton, &QRadioButton::pressed, this, [this]() {updateTerrain(Terrain::desert); });
	connect(ui.forestButton, &QRadioButton::pressed, this, [this]() {updateTerrain(Terrain::forest); });
	connect(ui.plainsButton, &QRadioButton::pressed, this, [this]() {updateTerrain(Terrain::plains); });
	
	connect(ui.locationName, &QLineEdit::textChanged,    this, [this]() {locationLabel->updateLabelName(ui.locationName->text()); });
	connect(ui.goldCount, &QSpinBox::valueChanged,       this, [this]() {locationLabel->updateLabelGold(ui.goldCount->value()); });
	connect(ui.knowledgeCount, &QSpinBox::valueChanged,  this, [this]() {locationLabel->updateLabelKnowledge(ui.knowledgeCount->value()); });
	connect(ui.populationCount, &QSpinBox::valueChanged, this, [this]() {locationLabel->updateLabelPopulation(ui.populationCount->value()); });
	connect(ui.defenceCount, &QSpinBox::valueChanged,    this, [this]() {locationLabel->updateLabelDefence(ui.defenceCount->value()); });

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

	locationLabel->updateLabelName(ui.locationName->text());
	locationLabel->updateLabelGold(ui.goldCount->value());
	locationLabel->updateLabelKnowledge(ui.knowledgeCount->value());
	locationLabel->updateLabelPopulation(ui.populationCount->value());
	locationLabel->updateLabelDefence(ui.defenceCount->value());

	fileGenerator = new FileGenerator(&hexagonsGraph, locationLabel);
	connect(ui.loadSavedLocation, &QPushButton::pressed, this, &HexagonGenerator::loadSavedLocation);
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

void HexagonGenerator::updateTerrain(Terrain terrain)
{
	scene->updateTerrain(terrain);
}

void HexagonGenerator::loadSavedLocation()
{
	QString savedLocationFile = QFileDialog::getOpenFileName(
		nullptr, "Open File", QDir::currentPath() + "\\generated_locations", "CSV Files (*.csv)");

	QString content;
	if (!savedLocationFile.isEmpty()) {
		QFile file(savedLocationFile);

		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			content = in.readAll();

			file.close();
		}
		else {
			return;
		}
	}
	else {
		return;
	}

	QStringList fields = content.split(',');
	// example output:
	// Blah Blah,7,desert,0,0,0,0,340.000000,376.000000,0,0.000000,-3,3,-2,2,-1,1,0,0,1,0,2,0,2,1
	
	ui.locationName->setText(fields[0]);

	int hexagonsCount = fields[1].toInt();
	ui.hexagonsCount->blockSignals(true);
	ui.hexagonsCount->setValue(hexagonsCount);
	ui.hexagonsCount->blockSignals(false);
	
	switch (stringToTerrain(fields[2].toStdString())) {
	case Terrain::desert:
		ui.desertButton->animateClick();
		break;
	case Terrain::forest:
		ui.forestButton->animateClick();
		break;
	case Terrain::plains:
		ui.plainsButton->animateClick();
		break;
	}

	ui.goldCount->setValue(fields[3].toInt());
	ui.knowledgeCount->setValue(fields[4].toInt());
	ui.populationCount->setValue(fields[5].toInt());
	ui.defenceCount->setValue(fields[6].toInt());

	locationLabel->setTransform(fields[7].toFloat(), fields[8].toFloat(), fields[9].toInt(), fields[10].toFloat());

	std::vector<std::pair<int, int>> newGraph;
	newGraph.reserve(hexagonsCount);
	for (int i = 0; i < hexagonsCount * 2; i+=2) {
		newGraph.emplace_back(fields[11 + i].toInt(), fields[11 + i + 1].toInt());
	}
	hexagonsGraph.setGraph(newGraph);
	scene->updateHexColors();
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

	int hexHeight = 33; // [mm]
	fileGenerator->generateFiles(ui.locationName->text(), hexHeight, currentTerrain);
}

void HexagonGenerator::updateHexagonsCount(int delta) {
	ui.hexagonsCount->blockSignals(true);
	ui.hexagonsCount->setValue(ui.hexagonsCount->value() + delta);
	ui.hexagonsCount->blockSignals(false);
}
