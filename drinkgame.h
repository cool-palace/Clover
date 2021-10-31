#ifndef DRINKGAME_H
#define DRINKGAME_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <button.h>
#include <QMediaPlayer>
#include "QCursor"
#include <QDebug>
#include "game.h"

class Drink : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Drink(int id, QGraphicsItem * parent = nullptr);
    virtual ~Drink() = default;
    static int last_taste;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

public slots:
    int taste();

signals:
    void degustation(int, int);

private:
    int drink_taste;
    QPointF m_shiftMouseCoords;
};

class DrinkGame : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    DrinkGame(QGraphicsItem* parent = nullptr);
    virtual ~DrinkGame();

public slots:
    void checkAnswer();

signals:
    void result(int, int);

private:
    QBrush brush;
    Drink * drinks[5];
    QGraphicsRectItem * drink_slots[5];
    QGraphicsTextItem * number[5];
    QGraphicsRectItem * cond_bg;
    QGraphicsTextItem * conditions;
    Button * confirmButton;
    QMediaPlayer * correctSound;
    QMediaPlayer * wrongSound;
};

#endif // DRINKGAME_H
