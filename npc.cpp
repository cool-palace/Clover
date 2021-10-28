#include "npc.h"
#include "game.h"
#include <QtMath>
extern Game * game;

GameObject::GameObject(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {}

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
//    int x = game->scene->width()/2 - diameter/2;
//    int y = game->scene->height()/2 - diameter/2;
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
