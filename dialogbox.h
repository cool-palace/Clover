#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QTimer>
#include "speechline.h"

class DialogBox : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    DialogBox(QGraphicsItem * parent = nullptr);
    virtual ~DialogBox();
    static int start;
    static int end;
    void keyPressEvent(QKeyEvent * event);

public slots:
    void getBox(int start, int end);
    void dialog_recharge();

signals:

private:
    QGraphicsTextItem * line;
    QGraphicsPixmapItem * avatar;
    QString right_aligned_str = "<p style=\"text-align:right;\">%1</p>";
    QTimer * timer;
    bool ready_to_change = true;
};

#endif // DIALOGBOX_H
