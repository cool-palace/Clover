#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Bullet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Bullet(int dir, qreal size = 1, QGraphicsItem *parent = nullptr);
    virtual ~Bullet();
    enum directions {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    static int bullet_count;

public slots:
    void move();

signals:
    void hitTarget();
    void dialogCall(int start, int end);

private:
    directions direction = LEFT;
    QTimer * timer;
};

class BugBossBuzz : public QGraphicsTextItem
{
    Q_OBJECT
public:
    BugBossBuzz(QGraphicsItem *parent = nullptr);
    virtual ~BugBossBuzz();

public slots:
    void move();

signals:
    void hitTarget();

private:
    QTimer * timer;
    const QVector<QString> buzzs = {"<i>бззз</i>", "<i>жжжж</i>", "<i>шшшш</i>"};

};

#endif // BULLET_H
