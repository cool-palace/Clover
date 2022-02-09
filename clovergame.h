#ifndef CLOVERGAME_H
#define CLOVERGAME_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QFont>
#include <button.h>
#include <QMediaPlayer>
#include "QCursor"
#include <QDebug>

class Digit : public QGraphicsTextItem {
    Q_OBJECT
public:
    Digit(int digit, QGraphicsItem* parent = nullptr);
    ~Digit() override = default;
    int value();

public slots:
    void increase();
    void decrease();

private:
    int value_;
};

class CloverGame : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    CloverGame(QGraphicsItem * parent = nullptr);
    ~CloverGame() override;

public slots:
    void checkAnswer();

signals:
    void result(int, int);

private:
    QGraphicsPixmapItem * clover;
    QGraphicsRectItem * cond_bg;
    QGraphicsTextItem * conditions;
    QGraphicsRectItem * score_bg;
    QGraphicsTextItem * score;
    int digitsCount;
    Button ** upButtons;
    Button ** downButtons;
    Button * confirmButton;
    Digit ** digits;
    QString answer;
    QString score_str = "<p style=\"text-align:center;\">Зелёных листов: %1<br>Жёлтых листов: %2</p>";
    QMediaPlayer * correctSound;
    QMediaPlayer * wrongSound;
};

#endif // CLOVERGAME_H
