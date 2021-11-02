#include "drinkgame.h"
#include "game.h"

extern Game * game;

Drink::Drink(int id, QGraphicsItem * parent) : QObject(), QGraphicsPixmapItem(parent)
{
    QString str = ":/images/drink-small-%1.png";
    setPixmap(QPixmap(str.arg(id+1)));
    switch (id) {
    case 0:
        drink_taste = 4;
        break;
    case 1:
        drink_taste = 2;
        break;
    case 2:
        drink_taste = 1;
        break;
    case 3:
        drink_taste = 5;
        break;
    case 4:
        drink_taste = 3;
        break;
    }
    setAcceptHoverEvents(true);
    setZValue(10);
}

int Drink::last_taste = 0;

int Drink::taste() {
    return drink_taste;
}

void Drink::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    this->setPos(mapToScene(event->pos() + m_shiftMouseCoords));
}

void Drink::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    m_shiftMouseCoords = pos() - mapToScene(event->pos());
    setCursor(QCursor(Qt::ClosedHandCursor));
    Q_UNUSED(event);
}

void Drink::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
}

void Drink::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (last_taste == 0) {
        emit degustation(Game::energySeqStart,Game::energySeqStart);
    } else {
        int taste_diff = drink_taste - last_taste;
        emit degustation(taste_diff+Game::energySeqStart+5,taste_diff+Game::energySeqStart+5);
    }
    last_taste = drink_taste;
}

void Drink::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    setCursor(QCursor(Qt::OpenHandCursor));
}

void Drink::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    setCursor(QCursor(Qt::ArrowCursor));
}

DrinkGame::DrinkGame(QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/images/bg.jpg"));
    setZValue(3);

    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(255,174,201,150));

    QPen pen;
    QBrush pen_brush;
    pen_brush.setStyle(Qt::SolidPattern);
    pen_brush.setColor(QColor(255,255,255,150));
    pen.setBrush(pen_brush);

    cond_bg = new QGraphicsRectItem(this);
    cond_bg->setRect(50,30, 700, 100);
    cond_bg->setBrush(brush);
    cond_bg->setPen(pen);

    conditions = new QGraphicsTextItem(this);
    QString cond = "Попробуй энергетики двойным кликом и расположи их в порядке улучшения вкуса. При этом вкусовые ощущения от каждого энергетика будут зависеть от того,<br>какой из них ты попробовала последним.";
    QString str1 = "<p style=\"text-align:center;\">%1</p>";
    conditions->setHtml(str1.arg(cond));
    conditions->setFont({"Calibri", 14});
    conditions->setTextWidth(700);
    conditions->setPos(50,40);

    for (int i = 0; i < 5; ++i) {
        int offset = 120;
        drinks[i] = new Drink(i, this);
        drinks[i]->setPos(i*120+offset,150);

        offset = 40;
        drink_slots[i] = new QGraphicsRectItem(this);
        drink_slots[i]->setRect(i*150+offset,300+20, 120, 180);
        drink_slots[i]->setBrush(brush);
        drink_slots[i]->setPen(pen);

        number[i] = new QGraphicsTextItem(this);
        QString str = "<p style=\"text-align:center;\"><i>#%1</i></p>";
        number[i]->setHtml(str.arg(5 - i));
        number[i]->setFont({"Calibri", 24});
        number[i]->setTextWidth(120);
        number[i]->setPos(i*150+offset,320+60);

        connect(drinks[i],SIGNAL(degustation(int, int)),game->dialogbox,SLOT(getBox(int, int)));
    }

    confirmButton = new Button("Готово",this);
    confirmButton->setPos(boundingRect().width()/2 - confirmButton->boundingRect().width()/2, 525);

    connect(confirmButton,SIGNAL(clicked()),this,SLOT(checkAnswer()));

    correctSound = new QMediaPlayer();
    correctSound->setMedia(QUrl("qrc:/sounds/correct.wav"));

    wrongSound = new QMediaPlayer();
    wrongSound->setMedia(QUrl("qrc:/sounds/wrong.wav"));
}

DrinkGame::~DrinkGame() {
    delete cond_bg;
    delete conditions;
    delete confirmButton;
    delete correctSound;
    delete wrongSound;

    for (int i = 0; i < 5; ++i) {
        delete drinks[i];
        delete drink_slots[i];
        delete number[i];
    }
}

void DrinkGame::checkAnswer() {

    bool all_placed = true;

    for (int i = 0; i < 5; ++i) {
        QList<QGraphicsItem *> colliding_items = drink_slots[i]->collidingItems();
        for (int j = 0; j < colliding_items.size(); ++j) {
            if (colliding_items[j]->zValue() < 10) {
                colliding_items.removeAt(j);
                --j;
            }
        }
        if (colliding_items.size() == 0) {
            emit result(Game::energySeqStart+10,Game::energySeqStart+10);
            all_placed = false;
            return;
        } else if (colliding_items.size() > 1) {
            emit result(Game::energySeqStart+11,Game::energySeqStart+11);
            all_placed = false;
            return;
        }
    }

    if (!all_placed) return;

    if (drink_slots[0]->collidesWithItem(drinks[2])
            && drink_slots[1]->collidesWithItem(drinks[1])
            && drink_slots[2]->collidesWithItem(drinks[4])
            && drink_slots[3]->collidesWithItem(drinks[0])
            && drink_slots[4]->collidesWithItem(drinks[3])) {
        emit result(Game::energySeqStart+13,Game::energySeqStart+13);
        correctSound->play();
        return;
    }
    emit result(Game::energySeqStart+12,Game::energySeqStart+12);
    wrongSound->play();
}
