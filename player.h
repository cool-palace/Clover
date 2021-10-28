#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QMediaPlayer>

class Player : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Player(QGraphicsItem * parent = nullptr);
    virtual ~Player();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    void setMovable();
    void setImmobile();
    bool phones_are_on();
    enum directions {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

signals:
    void dialogCall(int start, int end);
    void goingOut();
    void goingIn();
    void phone_game_advance();

private:
    QMediaPlayer * bulletSound;
    bool isMovable = true;
    bool canShoot = false;
    bool phones_on = false;
    directions direction = LEFT;

};

#endif // PLAYER_H