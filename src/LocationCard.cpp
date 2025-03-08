#include "LocationCard.h"
#include "LocationLabel.h"

LocationCard::LocationCard() : QFrame(), labelScene()
{
	setStyleSheet("QFrame { background-color: rgba(0, 0, 0, 0); border: none; }");

	graphicsView = new QGraphicsView(this);
	graphicsView->setRenderHint(QPainter::TextAntialiasing, true);
	graphicsView->setRenderHint(QPainter::Antialiasing, true);
	graphicsView->setRenderHint(QPainter::SmoothPixmapTransform, true);
	graphicsView->setScene(&labelScene);

	backgroundItem = new QGraphicsPixmapItem();
	labelScene.addItem(backgroundItem);

	nameLabel = new QLabel(this);
	goldLabel = new QLabel(this);
	knowledgeLabel = new QLabel(this);
	populationLabel = new QLabel(this);
	defenceLabel = new QLabel(this);

	int fontId = QFontDatabase::addApplicationFont(":/HexagonGenerator/fonts/EB_Garamond/static/EBGaramond-ExtraBold.ttf");
	QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
	QFont font(fontFamily, 12);
	font.setStyleStrategy(QFont::PreferAntialias);

	nameLabel->setFont(font);
	goldLabel->setFont(font);
	knowledgeLabel->setFont(font);
	populationLabel->setFont(font);
	defenceLabel->setFont(font);

	QPalette palette;
	palette.setColor(QPalette::WindowText, Qt::black);
	nameLabel->setPalette(palette);
	goldLabel->setPalette(palette);
	knowledgeLabel->setPalette(palette);
	populationLabel->setPalette(palette);
	defenceLabel->setPalette(palette);

	nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	goldLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	knowledgeLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	populationLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	defenceLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	nameLabel->setVisible(true);
	goldLabel->setVisible(true);
	knowledgeLabel->setVisible(true);
	populationLabel->setVisible(true);
	defenceLabel->setVisible(true);
	backgroundItem->setVisible(true);
	graphicsView->show();
}

LocationCard::~LocationCard()
{
	delete backgroundItem;
}

LocationCard::LocationCard(LocationLabel* locationLabel, int targetWidthPx) :
	LocationCard()
{
	nameLabel->setText(locationLabel->nameLabel->text());
	goldLabel->setText(locationLabel->goldLabel->text());
	knowledgeLabel->setText(locationLabel->knowledgeLabel->text());
	populationLabel->setText(locationLabel->populationLabel->text());
	defenceLabel->setText(locationLabel->defenceLabel->text());

	scaleEverything(targetWidthPx);

	labelScene.setSceneRect(labelScene.itemsBoundingRect());
}

void LocationCard::scaleEverything(float width)
{
	//width = 6300;
	float height = width / 7.875; // 6300x800 original card pixmap

	backgroundItem->setPixmap(ResourceManager::getCard(width, height));
	width = backgroundItem->sceneBoundingRect().width();
	height = backgroundItem->sceneBoundingRect().height();

	setGeometry(0, 0, width, height);
	graphicsView->setGeometry(0, 0, width, height);
	labelScene.setSceneRect(0, 0, width, height);

	int textY = height / 8;
	int textHeight = height * 600 / 800;
	float numberWidth = width * 600 / 6300;
	float numberShift = width * 3600 / 6300;

	nameLabel->setGeometry(width * 300 / 6300, textY, width * 3000 / 6300, textHeight);
	goldLabel->setGeometry(      numberShift + 0 * numberWidth, textY, numberWidth, textHeight);
	knowledgeLabel->setGeometry( numberShift + 1 * numberWidth, textY, numberWidth, textHeight);
	populationLabel->setGeometry(numberShift + 2 * numberWidth, textY, numberWidth, textHeight);
	defenceLabel->setGeometry(   numberShift + 3 * numberWidth, textY, numberWidth, textHeight);

	QFont font = nameLabel->font();
	font.setPointSizeF(310 * textHeight / 600);
	nameLabel->setFont(font);
	goldLabel->setFont(font);
	knowledgeLabel->setFont(font);
	populationLabel->setFont(font);
	defenceLabel->setFont(font);
}
