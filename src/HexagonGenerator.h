#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QSpinBox.h>
#include <QButtonGroup.h>
#include <QRadioButton.h>
#include <QFrame.h>
#include <qlineedit.h>
#include <QCheckBox>
#include <QSlider>
#include <QFileDialog>

#include "ui_HexagonGenerator.h"

#include "HexagonsGraph.h"
#include "HexagonsScene.h"
#include "LocationLabel.h"
#include "CustomDial.h"
#include "FileGenerator.h"


class HexagonGenerator : public QMainWindow
{
	Q_OBJECT

public:
	HexagonGenerator(QWidget *parent = nullptr);
	void updateHexagonsCount(int delta);
	~HexagonGenerator();

private slots:
	void randomizeGraph();
	void updateTerrain(Terrain terrain);
	void startFileGeneration();
	void loadSavedLocation();

private:
	Ui::HexagonGeneratorClass ui;    
	HexagonsGraph hexagonsGraph;    
	CustomDial* dial;
	HexagonsScene* scene;
	LocationLabel* locationLabel;
	FileGenerator* fileGenerator;
};
