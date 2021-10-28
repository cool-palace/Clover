#include "player.h"
#include "game.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
//#include "bullet.h"

extern Game * game; // there is an external global object called game
int step = 24;

Player::Player(QGraphicsItem *parent): QGraphicsPixmapItem(parent){
    // set graphic
    setPixmap(QPixmap(":/images/player-left.png"));
    bulletSound = new QMediaPlayer();
    bulletSound->setMedia(QUrl("qrc:/sounds/bullet.wav"));
    bulletSound->setVolume(5);
}

Player::~Player() {
    delete bulletSound;
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

void Player::keyPressEvent(QKeyEvent *event){

    if (event->key() == Qt::Key_Space && !canShoot) {
        phones_on = true;
    }

//    if (event->key() == Qt::Key_Space && canShoot && Bullet::bullet_count < 10) {
//        if (game->progress == Game::OUTSIDE_EMPTINESS_DISCOVERED
//                || game->progress == Game::DEADMAN_REVIVED) direction = LEFT;

//        if (bulletSound->state() == QMediaPlayer::PlayingState){
//            bulletSound->setPosition(0);
//        } else if (bulletSound->state() == QMediaPlayer::StoppedState){
//            bulletSound->play();
//        }

//        Bullet * bullet = new Bullet(direction, bullet_size);

//        switch (direction) {
//        case UP:
//            bullet->setPos(x()+boundingRect().width()/2,y()-bullet->boundingRect().height());
//            break;
//        case DOWN:
//            bullet->setPos(x()+boundingRect().width()/2,y()+boundingRect().height());
//            break;
//        case LEFT:
//            bullet->setPos(x()-bullet->boundingRect().width(),y()+boundingRect().height()/2);
//            break;
//        case RIGHT:
//            bullet->setPos(x()+boundingRect().width(),y()+boundingRect().height()/2);
//            break;
//        }
//        game->scene->addItem(bullet);
//        return;
//    }

    // calculating new position

    QPointF diff = {0, 0};
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) {
        if (direction != LEFT) {
            setPixmap(QPixmap(":/images/player-left.png"));
            direction = LEFT;
        }

        if (x() > 0) {
            if (x()-step > 0) {
                diff.setX(-step);
            } else diff.setX(-x());
        }
    }
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D){
        if (direction != RIGHT) {
            setPixmap(QPixmap(":/images/player-right.png"));
            direction = RIGHT;
        }
        if (x() < game->scene->width() - boundingRect().width()*scale()) {
            if (x()+step < game->scene->width() - boundingRect().width()*scale()) {
                diff.setX(step);
            } else diff.setX(game->scene->width() - boundingRect().width()*scale() - x());
        }
    }
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W){
        direction = UP;
        if (y() > 0) {
            if (y()-step > 0) {
                diff.setY(-step);
            } else diff.setY(-y());
        }
    }
    else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S){
        direction = DOWN;
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


    for (int i = 0; i < child_items.size(); ++i) {
        for (int j = 0; j < colliding_items.size(); ++j) {
            if (colliding_items.at(j) == child_items.value(i) || colliding_items.value(j) == this
                    /*|| dynamic_cast<Bullet*>(colliding_items.value(i))*/) {
                colliding_items.removeAt(j);
                --j;
            }
        }
    }


    for (int i = 0, n = colliding_items.size(); i < n; ++i){
        if (dynamic_cast<GameObject*>(colliding_items.at(i))) {
            qDebug() << "npc detected";
            setImmobile();
            delete rect;
            GameObject * npc = dynamic_cast<GameObject*>(colliding_items.at(i));
            npc->interact();
            return;
        }

//        if (dynamic_cast<Deadman*>(colliding_items.at(i))) {
//            setImmobile();
//            switch (game->progress) {
//            case Game::START:
//                game->deadman->setPixmap(QPixmap(":/images/deadman-sprite.png"));
//                game->deadman->setPos(game->scene->width()/2 - game->deadman->boundingRect().width()/2, 100);
//                emit dialogCall(Game::deadmanSeq1Start+4,Game::deadmanSeq1Start+30);
//                break;
//            case Game::DOG_QUEST_COMPLETE:
//                emit dialogCall(Game::deadmanSeq2Start,Game::deadmanSeq2Start+4);
//                break;
//            case Game::UNICORN_QUEST_COMPLETE:
//                emit dialogCall(Game::deadmanSeq3Start,Game::deadmanSeq3Start);
//                break;
//            case Game::DANCE_QUEST_COMPLETE:
//                emit dialogCall(Game::deadmanSeq4Start,Game::deadmanSeq4Start);
//                break;
//            case Game::SNAKES_DEFEATED:
//                emit dialogCall(Game::deadmanSeq5Start,Game::deadmanSeq5Start+9);
//                break;
//            case Game::TREE_QUEST_COMPLETE:
//                emit dialogCall(Game::deadmanSeq6Start,Game::deadmanSeq6Start);
//                break;
//            case Game::PHILOPHOBE_QUEST_COMPLETE:
//                emit dialogCall(Game::deadmanSeq7Start,Game::deadmanSeq7Start+1);
//                break;
//            case Game::WITCH_DEFEATED:
//                emit dialogCall(Game::deadmanSeq8Start,Game::deadmanSeq8Start+16);
//                break;
//            }

//        } else if (dynamic_cast<Entrance*>(colliding_items.at(i))) {
//            if (game->progress == Game::DOG_QUEST_STARTED || game->progress == Game::SNAKE_FIGHT_STARTED
//                    || game->progress == Game::FIFTH_RIDDLE_SOLVED) {
//                setImmobile();
//            } else  {
//                emit goingIn();
//                return;
//            }

//        } else if (dynamic_cast<Cave*>(colliding_items.at(i))) {
//            setImmobile();

//        } else if (dynamic_cast<Portal*>(colliding_items.at(i))) {
//            if (game->progress == Game::FIFTH_RIDDLE_SOLVED) {
//                setImmobile();
//                emit dialogCall(Game::deadmanSeq7Start+3,Game::deadmanSeq7Start+25);
//            } else if (game->progress == Game::WITCH_DEFEATED) {
//                setImmobile();
//            } else if (game->progress == Game::DEADMANS_FAREWELL) {
//                game->displayMainMenu();
//                return;
//            }

//        } else if (dynamic_cast<Kids*>(colliding_items.at(i))) {
//            if (childItems().empty()) {
//                setImmobile();
//                emit dialogCall(Game::kidsSeqStart,Game::kidsSeqStart+10);
//            }

//        } else if (dynamic_cast<RedWhite*>(colliding_items.at(i))) {
//            setImmobile();
//            if (game->progress == Game::DOG_QUEST_STARTED) {
//                emit dialogCall(Game::kidsSeqStart+16,Game::kidsSeqStart+17);
//            }

//        } else if (dynamic_cast<Unicorn*>(colliding_items.at(i))) {
//            setImmobile();
//            if (game->progress == Game::FIRST_RIDDLE_SOLVED) {
//                emit dialogCall(Game::unicornSeqStart,Game::unicornSeqStart+3);
//            }

//        } else if (dynamic_cast<Couple*>(colliding_items.at(i))) {
//            setImmobile();
//            if (game->progress == Game::SECOND_RIDDLE_SOLVED) {
//                emit dialogCall(Game::coupleSeqStart,Game::coupleSeqStart+7);
//            }

//        } else if (dynamic_cast<Snake*>(colliding_items.at(i))) {
//            Snake * snake = dynamic_cast<Snake*>(colliding_items.at(i));
//            if (!snake->moving && !snake->dead) {
//                setImmobile();
//                emit dialogCall(Game::snakeSeqStart,Game::snakeSeqStart+14);
//            } else if (snake->dead) {
//                emit dialogCall(Game::snakeSeqStart+16+snake->id,Game::snakeSeqStart+16+snake->id);
//            }

//        } else if (dynamic_cast<Tree*>(colliding_items.at(i))) {
//            setImmobile();
//            if (game->progress == Game::AFTER_SNAKES_DIALOG_OVER) {
//                emit dialogCall(Game::kalinaSeqStart,Game::kalinaSeqStart+6);
//            }

//        } else if (dynamic_cast<Thinker*>(colliding_items.at(i))) {
//            setImmobile();
//            if (game->progress == Game::FOURTH_RIDDLE_SOLVED) {
//                emit dialogCall(Game::thinkerSeqStart,Game::thinkerSeqStart+10);
//            }

//        } else if (dynamic_cast<Exit*>(colliding_items.at(i))) {
//            emit goingOut();
//            return;
//        }
    }

    delete rect;

    if (isMovable) {
        setPos(newPos);
        if (game->scene->width() > 800) {
            game->currentViewPos += diff;
            QRectF newView = {game->currentViewPos.x(),game->currentViewPos.y(),800,600};
            game->setSceneRect(newView);
        }
    }

}

void Player::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space && phones_on) {
        phones_on = false;
    }
}
