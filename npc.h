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
    ~GameObject() override = default;

public slots:
    virtual bool interact() = 0;
};

class Enemy : public GameObject
{
    Q_OBJECT
public:
    Enemy(QGraphicsItem *parent = nullptr) {}
    ~Enemy() override = default;

public slots:
    virtual bool interact() override { return true; }
    virtual void shot() {}
};

class Bug : public Enemy
{
    Q_OBJECT
public:
    Bug(QGraphicsItem *parent = nullptr);
    ~Bug() override ;

public slots:
    virtual bool interact() override;
    void start();
    void move();
    void shot() override;

private:
    QTimer * timer;
    static int bugCount;
    bool dead = false;
};

class BugBoss : public Enemy
{
    Q_OBJECT
public:
    BugBoss(QGraphicsItem *parent = nullptr);
    ~BugBoss() override;

public slots:
    virtual bool interact() override;
    void start();
    void move();
    void shoot();
    void shot() override;
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
    ~Buzz() override;
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
    ~Flower() override;
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
    ~Gardener() override = default;

public slots:
    virtual bool interact() override;
};

class Mice : public GameObject
{
    Q_OBJECT
public:
    Mice(QGraphicsItem *parent = nullptr);
    ~Mice() override = default;

public slots:
    virtual bool interact() override;
};

class Passage : public GameObject
{
    Q_OBJECT
public:
    Passage(QGraphicsItem *parent = nullptr);
    ~Passage() override = default;

public slots:
    virtual bool interact() override;

signals:
    void game_advance();
};


#endif // NPC_H
