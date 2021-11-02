#include "npc.h"
#include "game.h"
#include <QtMath>
extern Game * game;

GameObject::GameObject(QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent) {}

Bug::Bug(QGraphicsItem *parent): GameObject(parent)
{
    setPixmap(QPixmap(":/images/bug-sprite.png"));

    // make/connect a timer to move() the bullet every so often
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    ++bugCount;
}

Bug::~Bug(){
    delete timer;
}

int Bug::bugCount = 0;

bool Bug::interact() {
    game->player->setImmobile();
    if (!dead) {
//        emit game->player->dialogCall(Game::snakeSeqStart,Game::snakeSeqStart+14);
    } else if (dead) {
//        emit game->player->dialogCall(Game::snakeSeqStart+16+snake->id,Game::snakeSeqStart+16+snake->id);
    }
    return true;
}

void Bug::start() {
    // start the timer
    timer->start(100);
//    moving = true;
}

void Bug::move() {
    QList<QGraphicsItem *> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i) {
        if (dynamic_cast<Player*>(colliding_items[i])){
//            emit game->player->dialogCall(Game::BugSeqStart+15,Game::BugSeqStart+15);
            setPos(x()+rand()%100, y()+rand()%100);
            return;
        }
    }

    int theta = rand() % 360;
    double x_diff = 20 * qSin(qDegreesToRadians((double) theta));
    double y_diff = 20 * qCos(qDegreesToRadians((double) theta));

    QPointF diff = {x_diff, y_diff};

    if (x()+x_diff < 0) {
        diff.setX(-x());
    }
    if (x()+x_diff > game->scene->width() - boundingRect().width()*scale()) {
        diff.setX(game->scene->width() - boundingRect().width()*scale() - x());
    }
    if (y()+y_diff < 0) {
        diff.setY(-y());
    }
    if (y()+y_diff > game->scene->height() - boundingRect().height()*scale()) {
        diff.setY(game->scene->height() - boundingRect().height()*scale() - y());
    }

    QPointF newPos = pos() + diff;
    setPos(newPos);
}

void Bug::shot() {
    if (!dead) {
        setPixmap(QPixmap(":/images/bug-shot.png"));
        dead = true;
        disconnect(timer,SIGNAL(timeout()),this,SLOT(move()));
        --bugCount;
        if (bugCount == 0) {
            for (int i = 0; i < 5; ++i) {
                delete game->bug[i];
            }

            game->boss = new BugBoss();
            game->boss->setPos(game->scene->width()-275, game->scene->height()/2 - game->boss->boundingRect().height()/2);
            game->scene->addItem(game->boss);

            game->dialogbox->getBox(Game::bugFightStart+5, Game::bugFightStart+7);
        }
    }
}

BugBoss::BugBoss(QGraphicsItem *parent): GameObject(parent)
{
    setPixmap(QPixmap(":/images/boss-sprite.png"));

    // make/connect a timer to move() the bullet every so often
    move_timer = new QTimer(this);
    connect(move_timer,SIGNAL(timeout()),this,SLOT(move()));
    shoot_timer = new QTimer(this);
    connect(shoot_timer,SIGNAL(timeout()),this,SLOT(shoot()));
    hurt_timer = new QTimer(this);
    hurt_timer->setSingleShot(true);
}

BugBoss::~BugBoss(){
    delete move_timer;
    delete shoot_timer;
    delete hurt_timer;
}

bool BugBoss::interact() {
    game->player->setPos(0, game->player->y());
    return true;
}

void BugBoss::start() {
    // start the timer
    move_timer->start(100);
    shoot_timer->start(1000);
}

void BugBoss::move() {
    if (direction_up) {
        if (pos().y() - step > 0) {
            setPos(pos().x(), pos().y() - step);
        } else {
            setPos(pos().x(), 0);
            direction_up = false;
        }
    } else if (pos().y() + step + boundingRect().height() < game->scene->height()) {
        setPos(pos().x(), pos().y() + step);
    } else {
        setPos(pos().x(), game->scene->height() - boundingRect().height());
        direction_up = true;
    }

    QList<QGraphicsItem *> colliding_items = collidingItems();

    for (int i = 0; i < colliding_items.size(); ++i) {
        if (dynamic_cast<Player*>(colliding_items.value(i))) {
            colliding_items[i]->setPos(0, colliding_items[i]->y());
        }
    }
}

void BugBoss::shoot() {
    BugBossBuzz * buzz = new BugBossBuzz();
    buzz->setPos(x()-20,y()+boundingRect().height()/2);
    game->scene->addItem(buzz);
}

void BugBoss::shot() {
    if (--lives > 0) {
        setPixmap(QPixmap(":/images/boss-hit.png"));
        hurt_timer->start(100);
        connect(hurt_timer,SIGNAL(timeout()),this,SLOT(recover()));
    } else {
        game->dialogbox->getBox(Game::bugFightStart+8,Game::bugFightStart+8);
        delete this;
    }
}

void BugBoss::recover() {
    setPixmap(QPixmap(":/images/boss-sprite.png"));
}

Buzz::Buzz(int id, QGraphicsItem *parent) : GameObject(parent) {
    setPixmap(QPixmap(":/images/bug.png"));

    QVector<QPointF> points = {{game->scene->width()/2, 100}, {game->scene->width()/2, 500 - boundingRect().height()},
                              {game->scene->width()/2, 100}, {200, 500 - boundingRect().height()},
                              {game->scene->width()/2, 500 - boundingRect().height()},
                              {200, game->scene->height()/2},
                              {game->scene->width() - 200, 100}};
    setPos(points[id+pow(2, level-1)]);

    wave = new QGraphicsEllipseItem(this);
    int radius = 10 + 100*id;
    wave->setRect(boundingRect().width()/2, boundingRect().height()/2, 2*radius, 2*radius);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(change()));
    timer->start(80);
}

Buzz::~Buzz() {
    delete timer;
    delete wave;
}

bool Buzz::interact() {
    return true;
};

int Buzz::level = 1;

int Buzz::current_level() {
    return level;
}

void Buzz::next_level() {
    ++level;
};

void Buzz::change() {
    int diameter = wave->rect().width() < game->scene->height() ? wave->rect().width() + 10 : 20;
    wave->setRect(boundingRect().width()/2 - diameter/2, boundingRect().height()/2 - diameter/2, diameter, diameter);
    if (wave->collidesWithItem(game->player)) {
        if (game->player->phones_are_on()) {
            int radius = 10;
            wave->setRect(boundingRect().width()/2, boundingRect().height()/2, 2*radius, 2*radius);
        } else {
            int pxPos = Buzz::current_level() < 3 ? 0 : game->scene->width()/2 - game->player->boundingRect().width()*game->player->scale()/2;
            int pyPos = Buzz::current_level() < 3 ? game->height()/2 - game->player->boundingRect().height()*game->player->scale()/2 : 0;
            game->player->setPos(pxPos, pyPos);
        }
    }
}

const QVector<QPointF> Flower::points = { {off_x, off_y},
                                          {off_x + width, off_y + height/2},
                                          {off_x + width/2, off_y + height/2},
                                          {off_x, off_y + height},
                                          {off_x_2nd, off_y + height},
                                          {off_x_2nd + width/2, off_y},
                                          {off_x_2nd + width, off_y + height},
                                          {off_x_3rd, off_y + height},
                                          {off_x_3rd + width, off_y + height/2},
                                          {off_x_3rd + width, off_y},
                                          {off_x_3rd, off_y + height/4},
                                          {off_x_3rd + width - 48, off_y + height/2},
                                          {off_x_3rd + width, off_y + height},
                                          {off_x_2nd + width/2, off_y + height/4*3},
};

const QString Flower::number = "0000453136017c";

int Flower::checked_count = 0;

Flower::Flower(int id, QGraphicsItem *parent): GameObject(parent), value(number[id]) {
    if (id < 13) {
        QString str = ":/images/flower-%1.png";
        setPixmap(QPixmap(str.arg(id % 5 + 1)));
    } else setPixmap(QPixmap(":/images/flower-c.png"));

    setScale(2);

    QPointF offset = {-boundingRect().width()/2, -boundingRect().height()/2};
    setPos(points[id] + offset);
    setZValue(1);

    label = new QGraphicsTextItem(this);
    label->setPlainText(value);
    label->setFont({"Calibri", 8});
    label->setPos(0, -16);
    label->hide();

    if (id > 0) hide();
}

Flower::~Flower() {
    delete label;
}

bool Flower::interact() {
    if (!checked) {
        game->player->setImmobile();
        checked = true;
        ++checked_count;
        if (value == 'c') {
            show_path();
        } else {
            label->show();
            emit show_next();
        }

        switch (checked_count) {
        case 1:
            game->dialogbox->getBox(Game::cloverSeqStart+5,Game::cloverSeqStart+7);
            break;
        case 3:
            game->dialogbox->getBox(Game::cloverSeqStart+8,Game::cloverSeqStart+8);
            break;
        case 4:
            game->dialogbox->getBox(Game::cloverSeqStart+9,Game::cloverSeqStart+9);
            break;
        case 5:
            game->dialogbox->getBox(Game::cloverSeqStart+10,Game::cloverSeqStart+10);
            break;
        case 10:
            game->dialogbox->getBox(Game::cloverSeqStart+11,Game::cloverSeqStart+11);
            break;
        case 13:
            game->dialogbox->getBox(Game::cloverSeqStart+12,Game::cloverSeqStart+13);
            break;
        case 14:
            game->dialogbox->getBox(Game::cloverSeqStart+14,Game::cloverSeqStart+21);
            break;
        default:
            break;
        }
    }
    return true;
}

void Flower::appear() {
    show();
}

void Flower::show_path() {
    QPointF offset = {8, 8};
    QPointF start = Flower::points[0] + offset;

    QPainterPath myPath;
    myPath.moveTo(start);
    myPath.cubicTo({Flower::off_x+Flower::width+72, Flower::off_y+8},
                   {Flower::off_x+Flower::width+72, Flower::off_y+8+Flower::height},
                   Flower::points[3] + offset);
    myPath.moveTo(Flower::points[2] + offset);
    myPath.lineTo(Flower::points[1] + offset);
    myPath.moveTo(Flower::points[4] + offset);
    myPath.lineTo(Flower::points[5] + offset);
    myPath.lineTo(Flower::points[6] + offset);
    myPath.moveTo(Flower::points[7] + offset);
    myPath.lineTo(Flower::points[8] + offset);
    myPath.lineTo(Flower::points[9] + offset);
    myPath.cubicTo({Flower::off_x_3rd-48, Flower::off_y+8},
                   {Flower::off_x_3rd-48, Flower::off_y+8+Flower::height/2},
                   Flower::points[8] + offset);
    myPath.lineTo(Flower::points[12] + offset);

    QGraphicsPathItem * path = new QGraphicsPathItem(myPath);
    QPen myPen(QColor(19, 136, 8), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    path->setPen(myPen);
    path->show();
    game->scene->addItem(path);
}

Gardener::Gardener(QGraphicsItem *parent) : GameObject(parent) {
    setPixmap(QPixmap(":/images/gardener.png"));
}

bool Gardener::interact() {
    game->player->setImmobile();
    switch (game->progress) {
    case Game::START:
//        game->deadman->setPixmap(QPixmap(":/images/deadman-sprite.png"));
//        game->deadman->setPos(game->scene->width()/2 - game->deadman->boundingRect().width()/2, 100);



        //Исправить
        game->dialogbox->getBox(Game::gardenerSeqStart+4,Game::gardenerSeqStart+5);
        break;
    case Game::INTRO_COMPLETE:

        break;
    case Game::PHONES_GAME_STARTED:
//        emit dialogCall(Game::deadmanSeq2Start,Game::deadmanSeq2Start+4);
        break;
    case Game::PHONES_GAME_COMPLETE:
//        emit dialogCall(Game::deadmanSeq3Start,Game::deadmanSeq3Start);
        break;
    case Game::ENERGY_DRINKS_GAME_COMPLETE: case Game::BUGS_FIGHT_STARTED:
//        emit dialogCall(Game::deadmanSeq4Start,Game::deadmanSeq4Start);
        break;
    case Game::BUGS_DEFEATED:
//        emit dialogCall(Game::deadmanSeq5Start,Game::deadmanSeq5Start+9);
        break;
    case Game::CLOVER_QUEST_COMPLETE:
//        emit dialogCall(Game::deadmanSeq6Start,Game::deadmanSeq6Start);
        break;
    }
    return true;
}

Mice::Mice(QGraphicsItem *parent) : GameObject(parent) {
    setPixmap(QPixmap(":/images/mice-sprite.png"));
    setPos(game->scene->width()/2 - boundingRect().width()/2, game->scene->height()/12);
}

bool Mice::interact() {
    game->player->setImmobile();
    if (game->progress < Game::ENERGY_DRINKS_GAME_COMPLETE) {
        game->dialogbox->getBox(Game::miceSeqStart,Game::miceSeqStart+12);
    }
    return true;
}

Passage::Passage(QGraphicsItem *parent) : GameObject(parent) {
    if (game->progress >= Game::ENERGY_DRINKS_GAME_COMPLETE) {
        setPixmap(QPixmap(":/images/passage-3.png"));
    } else if (game->progress == Game::PHONES_GAME_STARTED && Buzz::current_level() == 2) {
        setPixmap(QPixmap(":/images/passage-2.png"));
    } else setPixmap(QPixmap(":/images/passage-1.png"));

    QPointF pos1 = {game->scene->width()-boundingRect().width(), game->scene->height()/2 - boundingRect().height()/2};
    QPointF pos2 = {game->scene->width()/2 - boundingRect().width()/2, game->scene->height()-boundingRect().height()};
    setPos(Buzz::current_level() != 2 ? pos1 : pos2);
}

bool Passage::interact() {
    switch (game->progress) {
    case Game::INTRO_COMPLETE:
        game->dialogbox->getBox(Game::phonesSeqStart, Game::phonesSeqStart+1);
        game->progress = Game::PHONES_GAME_STARTED;
        return false;
        break;
    case Game::PHONES_GAME_STARTED:
        if (Buzz::current_level() < 3) {
            Buzz::next_level();
        } else {
            game->progress = Game::PHONES_GAME_COMPLETE;
        }
        break;
    default:
        break;
    }
    emit game_advance();

    return false;
}
