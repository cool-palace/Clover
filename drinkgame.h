#ifndef DRINKGAME_H
#define DRINKGAME_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <button.h>
#include <QMediaPlayer>
#include "QCursor"
#include <QDebug>

class Drink : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Drink(int id, QGraphicsItem * parent = nullptr);
    ~Drink() override = default;
    static int last_taste;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

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
    ~DrinkGame() override;

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
