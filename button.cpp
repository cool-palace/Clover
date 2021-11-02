#include "button.h"
#include <QGraphicsTextItem>
#include <QFont>
#include <QDebug>

Button::Button(QString name, QGraphicsItem *parent): QGraphicsRectItem(parent){
    // draw the rect
    //QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    text = new QGraphicsTextItem(name,this);

    if (name != "Больше" && name != "Меньше" && name != "Готово" && name != "Сбросить" && name != "Понятно") {
        setRect(0,0,200,50);
        // draw the text
        QFont titleFont("Calibri",16);
        text->setFont(titleFont);
    } else {
        setRect(0,0,100,30);

        // draw the text
        QFont titleFont("Calibri",12);
        text->setFont(titleFont);
    }

    brush.setColor(Qt::darkYellow);
    setBrush(brush);

    int xPos = rect().width()/2 - text->boundingRect().width()/2;
    int yPos = rect().height()/2 - text->boundingRect().height()/2;
    text->setPos(xPos,yPos);

    // allow responding to hover events
    setAcceptHoverEvents(true);
}

Button::~Button() {
    delete text;
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event){
    emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    // change color to cyan
    brush.setColor(Qt::yellow);
    setBrush(brush);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    // change color to dark cyan
    brush.setColor(Qt::darkYellow);
    setBrush(brush);
}
