#include "bullet.h"
#include "npc.h"
#include <QDebug>
#include "game.h"

extern Game * game;

Bullet::Bullet(int dir, qreal size, QGraphicsItem *parent): QObject(), QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/images/bullet.png"));
    setScale(size);

    direction = static_cast<directions>(dir);

    // make/connect a timer to move() the bullet every so often
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));

    // start the timer
    timer->start(50);
    ++bullet_count;
    show();
}

Bullet::~Bullet() {
    delete timer;
    --bullet_count;
};

void Bullet::move(){
    int step = 15;

    switch (direction) {
    case UP:
        setPos(x(),y()-step);
        break;
    case DOWN:
        setPos(x(),y()+step);
        break;
    case LEFT:
        setPos(x()-step,y());
        break;
    case RIGHT:
        setPos(x()+step,y());
        break;
    }

    QList<QGraphicsItem *> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i) {
        if (dynamic_cast<Bug*>(colliding_items[i])){
            Bug * bug = dynamic_cast<Bug*>(colliding_items[i]);
            bug->shot();
            delete this;
            return;
        } else if (dynamic_cast<BugBoss*>(colliding_items[i])) {
            BugBoss * bug_boss = dynamic_cast<BugBoss*>(colliding_items[i]);
            bug_boss->shot();
            delete this;
            return;
        } else if (dynamic_cast<BugBossBuzz*>(colliding_items[i])) {
            delete colliding_items[i];
            delete this;
            return;
        }
    }

    if (x() - boundingRect().width() < 0 || x() > game->scene->width()
            || y() < 0 || y() > game->scene->height()) {
        delete this;
    }
}

int Bullet::bullet_count = 0;

BugBossBuzz::BugBossBuzz(QGraphicsItem *parent): QGraphicsTextItem(parent)
{
    setDefaultTextColor(Qt::black);
    setFont({"Comic Sans", 16});
    setHtml(buzzs[rand()%3]);

    // make/connect a timer to move() the bullet every so often
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));

    // start the timer
    timer->start(50);
    show();
}

BugBossBuzz::~BugBossBuzz() {
    delete timer;
};

void BugBoss::interact() {

}

void BugBossBuzz::move(){
    int step = -15;

    setPos(x()+step,y());

    QList<QGraphicsItem *> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i) {
        if (dynamic_cast<Player*>(colliding_items[i])){
            Player* player = dynamic_cast<Player*>(colliding_items[i]);
            player->shot();
            delete this;
            return;
        }
    }

    if (x() - boundingRect().width() < 0 || x() > game->scene->width()
            || y() < 0 || y() > game->scene->height()) {
        delete this;
    }
}
