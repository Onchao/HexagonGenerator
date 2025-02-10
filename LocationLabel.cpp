#include "LocationLabel.h"

LocationLabel::LocationLabel(QGraphicsScene* scene) : QFrame(), labelScene()
{
	setStyleSheet("QFrame { background-color: rgba(0, 0, 0, 0); border: none; }");
	proxy = scene->addWidget(this);

	graphicsView = new QGraphicsView(this);
	graphicsView->setRenderHint(QPainter::TextAntialiasing, true);
	graphicsView->setRenderHint(QPainter::Antialiasing, true);
	graphicsView->setRenderHint(QPainter::SmoothPixmapTransform, true);
	graphicsView->setScene(&labelScene);

	centerDot = new QGraphicsEllipseItem(0, 0, centerDotSize, centerDotSize);
	centerDot->setBrush(Qt::red);
	centerDot->setPen(Qt::NoPen);
	backgroundItem = new QGraphicsPixmapItem();

	labelScene.addItem(backgroundItem);
	labelScene.addItem(centerDot);

	nameLabel = new QLabel(this);
	goldLabel = new QLabel(this);
	knowledgeLabel = new QLabel(this);
	populationLabel = new QLabel(this);
	defenceLabel = new QLabel(this);

	QFont font("Rubik", 12, QFont::Bold);
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

	nameLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
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

LocationLabel::~LocationLabel()
{
	delete centerDot;
	delete backgroundItem;
}

LocationLabel::LocationLabel(QGraphicsScene* scene, LocationLabel* other, int targetPixmapSize, bool simple) :
	LocationLabel(scene)
{
	nameLabel->setText(other->nameLabel->text());
	goldLabel->setText(other->goldLabel->text());
	knowledgeLabel->setText(other->knowledgeLabel->text());
	populationLabel->setText(other->populationLabel->text());
	defenceLabel->setText(other->defenceLabel->text());

	centerDot->hide();

	scaleEverything(other->currentScale * targetPixmapSize / 800);
	proxy->setPos({ (other->proxy->pos().x()) * targetPixmapSize / 800,
				(other->proxy->pos().y()) * targetPixmapSize / 800 });
	// scaling no longer needed
	//if (!simple) {
	//	scaleEverything(other->currentScale * targetPixmapSize / 800);
	//	proxy->setPos({ (other->proxy->pos().x()) * targetPixmapSize / 800,
	//				(other->proxy->pos().y()) * targetPixmapSize / 800 });
	//}
	//else {
	//	float additionalScale = 1;

	//	scaleEverything(other->currentScale * targetPixmapSize / 800);
	//	float originalWidth = proxy->boundingRect().width();
	//	float origalHeight = proxy->boundingRect().height();

	//	scaleEverything(additionalScale * other->currentScale * targetPixmapSize / 800);
	//	float newWidth = proxy->boundingRect().width();
	//	float newHeight = proxy->boundingRect().height();

	//	float additionalShiftX = (originalWidth - newWidth) / 2;
	//	float additionalShiftY = (origalHeight - newHeight) / 2;

	//	proxy->setPos({ 
	//		(other->proxy->pos().x()) * targetPixmapSize / 800 + additionalShiftX,
	//		(other->proxy->pos().y()) * targetPixmapSize / 800 + additionalShiftY });
	//}
	
	proxy->setRotation(other->proxy->rotation());
	labelScene.setSceneRect(labelScene.itemsBoundingRect());
}

void LocationLabel::updateText(const QString& name, int gold, int knowledge, int population, int defence)
{
	nameLabel->setText(name);
	goldLabel->setText(QString::number(gold));
	knowledgeLabel->setText(QString::number(knowledge));
	populationLabel->setText(QString::number(population));
	defenceLabel->setText(QString::number(defence));
}

void LocationLabel::setRotation(int angle)
{
	proxy->setRotation(angle);
}

void LocationLabel::showCenterDot(bool visible)
{
	visible ? centerDot->show() : centerDot->hide();
}

void LocationLabel::showLabel(bool visible)
{
	nameLabel->setVisible(visible);
	goldLabel->setVisible(visible);
	knowledgeLabel->setVisible(visible);
	populationLabel->setVisible(visible);
	defenceLabel->setVisible(visible);
	backgroundItem->setVisible(visible);
}


void LocationLabel::setSize(int size)
{
	if (size >= 0 && size < 5) {		
		currentScale = scales[size];
	}
	scaleEverything(currentScale);
}

/*
NOTE: slight shifts in position are caused by integer values of geometry like this->geometry().x()
it is possible to avoid them by using something like the following code

	int maxWidth = 3000;
	int maxHeight = maxWidth / 10 * 4;
	float width = maxWidth * scale;
	float height = maxHeight * scale;

	static float prevX = this->geometry().x();
	static float prevY = this->geometry().y();
	static float prevWidth = this->geometry().width();
	static float prevHeight = this->geometry().height();

	float newX = prevX + (prevWidth - width) / 2;
	float newY = prevY + (prevHeight - height) / 2;
	float newWidth = width;
	float newHeight = height;

	setGeometry(newX, newY, newWidth, newHeight);

	prevX = newX;
	prevY = newY;
	prevWidth = newWidth;
	prevHeight = newHeight;
*/
//
void LocationLabel::scaleEverything(float scale)
{
	int maxWidth = 3000;
	int maxHeight = maxWidth / 10 * 4;
	int width = maxWidth * scale;
	int height = maxHeight * scale;

	int prevWidth = this->geometry().width();
	int prevHeight = this->geometry().height();

	setGeometry(
		this->geometry().x() + (prevWidth - width) / 2, 
		this->geometry().y() + (prevHeight - height) / 2,
		width, height);

	graphicsView->setGeometry(0, 0, width, height);
	labelScene.setSceneRect(0, 0, width, height);
	proxy->setTransformOriginPoint(width / 2, height / 2);

	backgroundItem->setPixmap(ResourceManager::getLabel(width, height));
	centerDot->setPos(width / 2 - centerDotSize / 2, height / 2 - centerDotSize / 2);

	nameLabel->setGeometry(0, 0, width, height / 2);
	goldLabel->setGeometry(width / 10 + 0 * width / 5, height / 2, width / 5, height / 2);
	knowledgeLabel->setGeometry(width / 10 + 1 * width / 5, height / 2, width / 5, height / 2);
	populationLabel->setGeometry(width / 10 + 2 * width / 5, height / 2, width / 5, height / 2);
	defenceLabel->setGeometry(width / 10 + 3 * width / 5, height / 2, width / 5, height / 2);

	QFont font = nameLabel->font();
	font.setPointSizeF(300 * scale);
	nameLabel->setFont(font);
	goldLabel->setFont(font);
	knowledgeLabel->setFont(font);
	populationLabel->setFont(font);
	defenceLabel->setFont(font);
}

void LocationLabel::setCenterPosition(QPointF pos)
{
	proxy->setPos(pos.x() - labelScene.width() / 2, pos.y() - labelScene.height() / 2);
}

void LocationLabel::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);  // Enable antialiasing for the frame
	painter.setRenderHint(QPainter::TextAntialiasing, true); // Enable text antialiasing for the frame

	QFrame::paintEvent(event);
}

std::string LocationLabel::printInfo() {
	QString info =
		goldLabel->text() + "," 
		+ knowledgeLabel->text() + ","
		+ populationLabel->text() + ","
		+ defenceLabel->text();
	return info.toStdString();
}
