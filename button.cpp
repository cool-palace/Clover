#include "button.h"
#include <QGraphicsTextItem>
#include <QFont>
#include <QDebug>

Button::Button(QString name, QGraphicsItem *parent): QGraphicsRectItem(parent) {
    brush.setStyle(Qt::SolidPattern);

    text = new QGraphicsTextItem(name,this);

    if (name != "Больше" && name != "Меньше" && name != "Готово" && name != "Сбросить" && name != "Понятно") {
        setRect(0,0,200,50);
        QFont titleFont("Calibri",16);
        text->setFont(titleFont);
    } else {
        setRect(0,0,100,30);
        QFont titleFont("Calibri",12);
        text->setFont(titleFont);
    }

    brush.setColor(Qt::darkYellow);
    setBrush(brush);

    int xPos = static_cast<int>(rect().width()/2 - text->boundingRect().width()/2);
    int yPos = static_cast<int>(rect().height()/2 - text->boundingRect().height()/2);
    text->setPos(xPos,yPos);
    setAcceptHoverEvents(true);
}

Button::~Button() {
    delete text;
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
    emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event);
    brush.setColor(Qt::yellow);
    setBrush(brush);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event);
    brush.setColor(Qt::darkYellow);
    setBrush(brush);
}
