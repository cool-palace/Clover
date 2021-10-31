#include "npc.h"
#include "game.h"
#include <QtMath>
extern Game * game;

GameObject::GameObject(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {}

Bug::Bug(QGraphicsItem *parent): QObject(), GameObject(parent)
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

void Bug::interact() {
    if (!dead) {
//        emit game->player->dialogCall(Game::snakeSeqStart,Game::snakeSeqStart+14);
    } else if (dead) {
//        emit game->player->dialogCall(Game::snakeSeqStart+16+snake->id,Game::snakeSeqStart+16+snake->id);
    }
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
//        moving = false;
        dead = true;
        disconnect(timer,SIGNAL(timeout()),this,SLOT(move()));
        --bugCount;
        if (bugCount == 0) emit game->player->dialogCall(Game::bugFightStart+1, Game::bugFightStart+1);
    }
}

BugBoss::BugBoss(QGraphicsItem *parent): QObject(), GameObject(parent)
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
    } else delete this;
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
    qDebug() << "buzz del";
}

void Buzz::interact() {};

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

Passage::Passage(int buzz_level, QGraphicsItem *parent) : GameObject(parent) {
    QString str = ":/images/passage-%1.png";
    setPixmap(QPixmap(str.arg((buzz_level-1) % 2 + 1)));

    QPointF pos1 = {game->scene->width()-5, game->scene->height()/2 - boundingRect().height()/2};
    QPointF pos2 = {game->scene->width()/2 - boundingRect().width()/2, game->scene->height()-5};
    setPos(buzz_level != 2 ? pos1 : pos2);
}

void Passage::interact() {
    if (Buzz::current_level() < 3) {
        Buzz::next_level();
        emit phone_game_advance();
    } else {
        emit phone_game_end();
    }
    return;
}
