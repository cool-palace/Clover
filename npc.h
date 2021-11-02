#ifndef NPC_H
#define NPC_H
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QDebug>
#include "bullet.h"

class GameObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    GameObject(QGraphicsItem *parent = nullptr);
    virtual ~GameObject() = default;

public slots:
    virtual bool interact() = 0;
};

class Bug : public GameObject
{
    Q_OBJECT
public:
    Bug(QGraphicsItem *parent = nullptr);
    virtual ~Bug();

public slots:
    virtual bool interact() override;
    void start();
    void move();
    void shot();

private:
    QTimer * timer;
    static int bugCount;
    bool dead = false;
//    bool moving = false;
};

class BugBoss : public GameObject
{
    Q_OBJECT
public:
    BugBoss(QGraphicsItem *parent = nullptr);
    virtual ~BugBoss();

public slots:
    virtual bool interact() override;
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

class Buzz : public GameObject
{
    Q_OBJECT
public:
    Buzz(int id, QGraphicsItem *parent = nullptr);
    virtual ~Buzz();
    static int current_level();

public slots:
    void change();
    virtual bool interact() override;
    static void next_level();

private:
    QGraphicsEllipseItem * wave;
    QTimer * timer;
    static int level;
};

class Flower : public GameObject
{
    Q_OBJECT
public:
    Flower(int id, QGraphicsItem *parent = nullptr);
    virtual ~Flower();
    static const QVector<QPointF> points;
    static const int width = 180;
    static const int height = 320;
    static const int space = 30;
    static const int off_x = 100;
    static const int off_y = 85;
    static const int off_x_2nd = off_x + width + space;
    static const int off_x_3rd = off_x + 2*width + 2*space;

signals:
    void show_next();

public slots:
    virtual bool interact() override;
    void appear();
    static void show_path();

private:
    bool checked = false;
    static int checked_count;
    static const QString number;
    const QChar value;
    QGraphicsTextItem * label;
};

class Gardener : public GameObject
{
    Q_OBJECT
public:
    Gardener(QGraphicsItem *parent = nullptr);
    virtual ~Gardener() = default;

public slots:
    virtual bool interact() override;
};

class Mice : public GameObject
{
    Q_OBJECT
public:
    Mice(QGraphicsItem *parent = nullptr);
    virtual ~Mice() = default;

public slots:
    virtual bool interact() override;
};

class Passage : public GameObject
{
    Q_OBJECT
public:
    Passage(QGraphicsItem *parent = nullptr);
    virtual ~Passage() = default;

public slots:
    virtual bool interact() override;

signals:
    void game_advance();
};


#endif // NPC_H
