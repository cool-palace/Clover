#ifndef NPC_H
#define NPC_H
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QDebug>
#include "bullet.h"

class GameObject : public QGraphicsPixmapItem
{
public:
    GameObject(QGraphicsItem *parent = nullptr);
    virtual ~GameObject() = default;
    virtual void interact() = 0;
};

class Bug : public QObject, public GameObject
{
    Q_OBJECT
public:
    Bug(QGraphicsItem *parent = nullptr);
    virtual ~Bug();

public slots:
    virtual void interact() override;
    void start();
    void move();
    void shot();

private:
    QTimer * timer;
    static int bugCount;
    bool dead = false;
//    bool moving = false;
};

class BugBoss : public QObject, public GameObject
{
    Q_OBJECT
public:
    BugBoss(QGraphicsItem *parent = nullptr);
    virtual ~BugBoss();

public slots:
    virtual void interact() override;
    void start();
    void move();
    void shoot();
    void shot();
    void recover();

private:
    bool dead = false;
    bool moving = false;
    int lives = 20;
    bool direction_up = true;
    int step = 20;
    QTimer * move_timer;
    QTimer * shoot_timer;
    QTimer * hurt_timer;
};

class Buzz : public QObject, public GameObject
{
    Q_OBJECT
public:
    Buzz(int id, QGraphicsItem *parent = nullptr);
    virtual ~Buzz();
    static int current_level();

public slots:
    void change();
    virtual void interact() override;
    static void next_level();

private:
    QGraphicsEllipseItem * wave;
    QTimer * timer;
    static int level;
};

class Passage : public QObject, public GameObject
{
Q_OBJECT
public:
    Passage(int id, QGraphicsItem *parent = nullptr);
    virtual ~Passage() = default;

public slots:
    virtual void interact() override;

signals:
    void phone_game_advance();
    void phone_game_end();
};


#endif // NPC_H
