#ifndef PHONESGAME_H
#define PHONESGAME_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QBrush>
#include <QTimer>
#include <QKeyEvent>
#include <QMediaPlayer>
#include "game.h"

class PhonesGame : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    PhonesGame(QGraphicsItem *parent = nullptr);
    virtual ~PhonesGame();
    void keyPressEvent(QKeyEvent * event);

public slots:
    void change();

signals:

private:
    QBrush brush;
    Player * player;
    QTimer * timer;
    QGraphicsEllipseItem * buzz;
    bool phones_on = false;

};

#endif // PHONESGAME_H
