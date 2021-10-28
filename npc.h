#ifndef NPC_H
#define NPC_H
#include <QGraphicsPixmapItem>
#include <QTimer>

class GameObject : public QGraphicsPixmapItem
{
public:
    GameObject(QGraphicsItem *parent = nullptr);
    virtual ~GameObject() = default;
    virtual void interact() = 0;
};

class Passage : public QObject, public GameObject
{
Q_OBJECT
public:
    Passage(int id, QGraphicsItem *parent = nullptr);
    virtual ~Passage() = default;
    virtual void interact() override;

signals:
    void phone_game_advance();
    void phone_game_end();
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
    void interact() override;
    static void next_level();

private:
    QGraphicsEllipseItem * wave;
    QTimer * timer;
    static int level;
};

#endif // NPC_H
