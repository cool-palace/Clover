#include "player.h"
#include "game.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>

extern Game * game;
int step = 24;

Player::Player(QGraphicsItem *parent): QGraphicsPixmapItem(parent) {
    setPixmap(QPixmap(":/images/player-right.png"));
    bulletSound = new QMediaPlayer();
    bulletSound->setMedia(QUrl("qrc:/sounds/bullet.wav"));
    bulletSound->setVolume(5);
    setZValue(2);
}

Player::~Player() {
    delete bulletSound;
}

void Player::enable_shooting() {
    canShoot = true;
}

void Player::disable_shooting() {
    canShoot = false;
}

void Player::setMovable() {
    isMovable = true;
}

void Player::setImmobile() {
    isMovable = false;
}

bool Player::phones_are_on() {
    return phones_on;
}

void Player::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Space) {
        if (!canShoot) {
            phones_on = true;
            if (direction == RIGHT) {
                setPixmap(QPixmap(":/images/player-phones-right.png"));
            } else setPixmap(QPixmap(":/images/player-phones-left.png"));
        } else if (Bullet::bullet_count < 10) {
            direction = RIGHT;

    //        if (bulletSound->state() == QMediaPlayer::PlayingState){
    //            bulletSound->setPosition(0);
    //        } else if (bulletSound->state() == QMediaPlayer::StoppedState){
    //            bulletSound->play();
    //        }

            Bullet * bullet = new Bullet(direction);

            switch (direction) {
            case UP:
                bullet->setPos(x()+boundingRect().width()/2,y()-bullet->boundingRect().height());
                break;
            case DOWN:
                bullet->setPos(x()+boundingRect().width()/2,y()+boundingRect().height());
                break;
            case LEFT:
                bullet->setPos(x()-bullet->boundingRect().width(),y()+boundingRect().height()/2);
                break;
            case RIGHT:
                bullet->setPos(x()+boundingRect().width(),y()+boundingRect().height()/2);
                break;
            }
            game->scene->addItem(bullet);
            return;
        }
    }

    // calculating new position
    QPointF diff = {0, 0};
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) {
        if (direction != LEFT) {
            set_direction(LEFT);
        }
        if (x() > 0) {
            if (x()-step > 0) {
                diff.setX(-step);
            } else diff.setX(-x());
        }
    } else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) {
        if (direction != RIGHT) {
            set_direction(RIGHT);
        }
        if (x() < game->scene->width() - boundingRect().width()*scale()) {
            if (x()+step < game->scene->width() - boundingRect().width()*scale()) {
                diff.setX(step);
            } else diff.setX(game->scene->width() - boundingRect().width()*scale() - x());
        }
    } else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) {
        if (y() > 0) {
            if (y()-step > 0) {
                diff.setY(-step);
            } else diff.setY(-y());
        }
    } else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S) {
        if (y() < game->scene->height() - boundingRect().height()*scale()) {
            if (y() + step < game->scene->height() - boundingRect().height()*scale() ) {
                diff.setY(step);
            } else diff.setY(game->scene->height() - boundingRect().height()*scale() - y());
        }
    }

    // collision detection
    setMovable();
    QPointF newPos = pos() + diff;
    QGraphicsRectItem * rect = new QGraphicsRectItem(newPos.x(), newPos.y(),
                                                     boundingRect().width()*scale(), boundingRect().height()*scale());
    game->scene->addItem(rect);
    rect->hide();

    QList<QGraphicsItem *> colliding_items = rect->collidingItems();
    QList<QGraphicsItem *> child_items = childItems();
    delete rect;

    for (int i = 0; i < child_items.size(); ++i) {
        for (int j = 0; j < colliding_items.size(); ++j) {
            if (colliding_items.at(j) == child_items.value(i) || colliding_items.value(j) == this
                    || dynamic_cast<Bullet*>(colliding_items.value(i))) {
                colliding_items.removeAt(j);
                --j;
            }
        }
    }

    for (int i = 0, n = colliding_items.size(); i < n; ++i) {
        GameObject * npc = dynamic_cast<GameObject*>(colliding_items.at(i));
        if (npc && !npc->interact()) {
            // interact() returns false on passages and true on NPCs
            return;
        }
    }

    if (isMovable && !phones_on) {
        setPos(newPos);
    }
}

void Player::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space && phones_on) {
        phones_on = false;
        if (direction == RIGHT) {
            setPixmap(QPixmap(":/images/player-right.png"));
        } else setPixmap(QPixmap(":/images/player-left.png"));
        setMovable();
    }
}

void Player::shot() {
    if (x() - 100 > 0) {
        setPos(x()-100, y());
    } else setPos(0, y());
}

void Player::set_direction(directions dir) {
    direction = dir;
    if (dir == LEFT) {
        setPixmap(QPixmap(":/images/player-left.png"));
    } else if (dir == RIGHT) {
        setPixmap(QPixmap(":/images/player-right.png"));
    }
}
