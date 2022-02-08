#include "dialogbox.h"
#include "game.h"
#include <QGraphicsScene>
#include <QRectF>
#include <QKeyEvent>
#include <QDebug>

extern Game * game; // there is an external global object called game

int DialogBox::start = 0;
int DialogBox::end = 0;

DialogBox::DialogBox(QGraphicsItem * parent) : QObject(), QGraphicsRectItem (parent) {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkBlue);

    setRect(0,450,800,150);
    setBrush(brush);
    setZValue(5);

    line = new QGraphicsTextItem(this);
    avatar = new QGraphicsPixmapItem(this);

    line->setDefaultTextColor(Qt::white);
    line->setFont({"Calibri", 16});
    line->setTextWidth(640);

    setEnabled(true);

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer,SIGNAL(timeout()),this,SLOT(dialog_recharge()));

    hide();
}

DialogBox::~DialogBox() {
    delete line;
    delete avatar;
    delete timer;
}

void DialogBox::dialog_recharge() {
    ready_to_change = true;
}

void DialogBox::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space && ready_to_change) {
        if (start == end) {
            hide();
            game->player->setFocus();
            switch (end) {
            case Game::gardenerSeqStart+4:
                game->setBackgroundBrush(QBrush(QImage(":/images/bg-start.png")));
                game->gardener->show();
                game->music->setCurrentIndex(1);
                game->current_music->setVolume(50);
                game->current_music->play();
                getBox(Game::gardenerSeqStart+5, Game::gardenerSeqStart+35);
                break;
            case Game::gardenerSeqStart+35:
                game->passage->show();
                game->progress = Game::INTRO_COMPLETE;
                break;

            case Game::phonesSeqStart+1:
                emit game->passage->game_advance();
            break;

            case Game::miceSeqStart+12:
                game->drink_game();
                break;

            case Game::energySeqStart+13:
                // Зыкрыть напитки
                delete game->drinkgame;
                game->progress = Game::ENERGY_DRINKS_GAME_COMPLETE;
                game->passage->show();
                game->save();
                getBox(Game::energySeqStart+14,Game::energySeqStart+15);
                break;

            case Game::bugFightStart+4:
                // Начало боя с жуками
                game->player->enable_shooting();
                for (int i = 0; i < 5; ++i) {
                    game->bug[i]->start();
                }
                game->progress = Game::BUGS_FIGHT_STARTED;
                break;

            case Game::bugFightStart+7: {
                // Битва с боссом
                game->boss->start();
                break;
            }
            case Game::bugFightStart+8:
                // Победа
                game->progress = Game::BUGS_DEFEATED;
                game->passage->show();
                game->save();
                getBox(Game::bugFightStart+9, Game::bugFightStart+10);
                break;

            case Game::cloverSeqStart+21:
                // Загадка клевера
                game->clover_game();
                break;

            case Game::cloverSeqStart+22:
                // Закрываем клевер
                game->music->setCurrentIndex(6);
                game->current_music->setVolume(50);
                game->current_music->play();
                game->progress = Game::CLOVER_QUEST_COMPLETE;
                game->save();
                game->passage->show();
                delete game->clovergame;
                getBox(Game::cloverSeqStart+23,Game::cloverSeqStart+32);
                break;
            case Game::cloverSeqStart+32:
                game->clover->hide();
                getBox(Game::cloverSeqStart+33,Game::cloverSeqStart+37);
                break;
            }
        } else {
            ++start;
            getBox(start, end);
        }
    }
}

void DialogBox::getBox(int in_start, int in_end) {
    ready_to_change = false;
    timer->start(750);

    start = in_start;
    end = in_end;

    int xPos = 0, yPos = 450;

    avatar->setPixmap(QPixmap(game->speech[start].speaker));

    if (game->speech[start].speaker.contains("player")) {
        avatar->setPos(xPos+650,yPos);
        line->setHtml(right_aligned_str.arg(game->speech[start].line));
        line->setPos(xPos+10,yPos+10);
    } else {
        avatar->setPos(xPos,yPos);
        line->setHtml(game->speech[start].line);
        line->setPos(xPos+150,yPos+10);
    }

    show();

    grabKeyboard();
    grabMouse();
}
