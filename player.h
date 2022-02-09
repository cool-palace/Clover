#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QMediaPlayer>

class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Player(QGraphicsItem * parent = nullptr);
    ~Player() override;

    enum directions {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

public slots:
    void keyPressEvent(QKeyEvent * event) override;
    void keyReleaseEvent(QKeyEvent * event) override;
    void setMovable();
    void setImmobile();
    bool phones_are_on();
    void shot();
    void enable_shooting();
    void disable_shooting();
    void set_direction(directions);

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
    directions direction = RIGHT;
};

#endif // PLAYER_H
