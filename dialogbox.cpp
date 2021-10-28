#include "dialogbox.h"
#include "game.h"
#include <QGraphicsScene>
#include <QRectF>
#include <QKeyEvent>
#include <QDebug>

extern Game * game; // there is an external global object called game

int DialogBox::start = 0;
int DialogBox::end = 0;

DialogBox::DialogBox(QGraphicsItem * parent) : QObject(), QGraphicsRectItem (parent)
{
    // set the brush
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkBlue);

    setRect(0,450,800,150);
    setBrush(brush);
    setZValue(1);

    line = new QGraphicsTextItem(this);
    avatar = new QGraphicsPixmapItem(this);

    line->setDefaultTextColor(Qt::white);
    line->setFont({"Comic Sans", 16});
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

void DialogBox::keyPressEvent(QKeyEvent *event){

    if (event->key() == Qt::Key_Space && ready_to_change) {

        if (start == end) {
            hide();
//            game->player->setFocus();
//            switch (end) {
//            case Game::deadmanSeq1Start+30:
//                game->exit->show();
//                game->progress = Game::INTRO_COMPLETE;
//                game->music->setCurrentIndex(1);
//                game->current_music->setVolume(50);
//                game->current_music->play();
//                break;

//            case Game::kidsSeqStart+10:
//                // Гуляем и ловим собак
//                game->progress = Game::DOG_QUEST_STARTED;
//                game->kids->setParentItem(game->player);
//                game->kids->setPos(60,9);
//                for (int i = 0; i < 1; ++i) {
//                    game->dog = new Dog();
//                    game->dog->setPos(300,2000);
//                    game->scene->addItem(game->dog);
//                }
//                break;

//            case Game::kidsSeqStart+15:
//                game->kb->show();
//                break;

//            case Game::kidsSeqStart+17: {
//                delete game->dog;
//                delete game->kids;
//                game->progress = Game::DOG_QUEST_COMPLETE;
//                game->save();
//                getBox(Game::kidsSeqStart+18,Game::kidsSeqStart+18);
//                break; }

//            case Game::deadmanSeq2Start+4: case Game::deadmanSeq3Start:
//                case Game::deadmanSeq4Start: case Game::deadmanSeq6Start:
//                case Game::deadmanSeq7Start+1:
//                // Открыть мел
//                emit chalkCall();
//                break;
//            case Game::deadmanSeq2Start+10:
//                // Закрыть загадку
//                game->exit->show();
//                delete game->riddlebox;
//                game->music->setCurrentIndex(1);
//                game->current_music->play();
//                switch (game->progress) {
//                case Game::DOG_QUEST_COMPLETE:
//                    game->progress = Game::FIRST_RIDDLE_SOLVED;
//                    game->save();
//                    getBox(Game::deadmanSeq2Start+11,Game::deadmanSeq2Start+13);
//                    break;
//                case Game::UNICORN_QUEST_COMPLETE:
//                    game->progress = Game::SECOND_RIDDLE_SOLVED;
//                    game->save();
//                    getBox(Game::deadmanSeq3Start+1,Game::deadmanSeq3Start+11);
//                    break;
//                case Game::DANCE_QUEST_COMPLETE:
//                    game->progress = Game::THIRD_RIDDLE_SOLVED;
//                    game->save();
//                    getBox(Game::deadmanSeq4Start+1,Game::deadmanSeq4Start+17);
//                    break;
//                case Game::TREE_QUEST_COMPLETE:
//                    game->progress = Game::FOURTH_RIDDLE_SOLVED;
//                    game->save();
//                    game->music->setCurrentIndex(6);
//                    game->current_music->setVolume(25);
//                    game->current_music->play();
//                    getBox(Game::deadmanSeq6Start+1,Game::deadmanSeq6Start+10);
//                    break;
//                case Game::PHILOPHOBE_QUEST_COMPLETE:
//                    game->progress = Game::FIFTH_RIDDLE_SOLVED;
//                    //game->save();
//                    game->deadman->setParentItem(game->player);
//                    game->deadman->setPos(game->player->boundingRect().width()+10,0);
//                    game->exit->hide();
//                    game->portal->show();
//                    game->music->setCurrentIndex(6);
//                    game->current_music->setVolume(25);
//                    game->current_music->play();
//                    getBox(Game::deadmanSeq7Start+2,Game::deadmanSeq7Start+2);
//                    break;
//                default:
//                    break;
//                }
//                break;

//            case Game::unicornSeqStart+3:
//                emit dyegameCall();
//                break;
//            case Game::unicornSeqStart+8:
//                // Закрыть красители
//                delete game->dyegame;
//                game->music->setCurrentIndex(2);
//                game->current_music->setVolume(50);
//                game->current_music->play();
//                game->scene->setSceneRect(0,0,Game::worldSize,Game::worldSize);
//                game->setSceneRect(game->currentViewPos.x(),game->currentViewPos.y(),800,600);
//                game->progress = Game::UNICORN_QUEST_COMPLETE;
//                game->save();
//                getBox(Game::unicornSeqStart+9,Game::unicornSeqStart+17);
//                break;

//            case Game::coupleSeqStart+7:
//                // Открыть танец
//                emit dancegameCall();
//                break;

//            case Game::coupleSeqStart+8:
//                // Закрыть танец
//                game->music->setCurrentIndex(2);
//                game->current_music->setVolume(50);
//                game->current_music->play();
//                disconnect(game->dancegame->timer,SIGNAL(timeout()),this,SLOT(game->dancegame->change()));
//                delete game->dancegame;
//                game->scene->setSceneRect(0,0,Game::worldSize,Game::worldSize);
//                game->setSceneRect(game->currentViewPos.x(),game->currentViewPos.y(),800,600);
//                game->progress = Game::DANCE_QUEST_COMPLETE;
//                game->save();
//                getBox(Game::coupleSeqStart+9,Game::coupleSeqStart+9);
//                break;

//            case Game::snakeSeqStart+14:
//                // Стреляем змей
//                game->music->setCurrentIndex(5);
//                game->current_music->setVolume(50);
//                game->current_music->play();
//                game->player->canShoot = true;
//                for (int i = 0; i < 2; ++i) {
//                    game->snake[i]->start();
//                }
//                game->progress = Game::SNAKE_FIGHT_STARTED;
//                break;

//            case Game::snakeSeqStart+18:
//                // Стреляем змей
//                game->music->setCurrentIndex(2);
//                game->current_music->setVolume(50);
//                game->current_music->play();
//                game->progress = Game::SNAKES_DEFEATED;
//                game->save();
//                break;

//            case Game::deadmanSeq5Start+9:
//                game->progress = Game::AFTER_SNAKES_DIALOG_OVER;
//                game->save();
//                break;

//            case Game::kalinaSeqStart+6:
//                emit snackgameCall();
//                break;

//            case Game::kalinaSeqStart+20:
//                // Зыкрыть сырки
//                delete game->snackgame;
//                game->music->setCurrentIndex(2);
//                game->current_music->setVolume(50);
//                game->current_music->play();
//                game->scene->setSceneRect(0,0,Game::worldSize,Game::worldSize);
//                game->setSceneRect(game->currentViewPos.x(),game->currentViewPos.y(),800,600);
//                game->progress = Game::TREE_QUEST_COMPLETE;
//                game->save();
//                getBox(Game::kalinaSeqStart+21,Game::kalinaSeqStart+21);
//                break;

//            case Game::thinkerSeqStart+10:
//                // Открыть сапёра
//                emit voltorbgameCall();
//                break;

//            case Game::thinkerSeqStart+11: case Game::thinkerSeqStart+12: case Game::thinkerSeqStart+13:
//                // Открыть сапёра
//                game->voltorbgame->reset();
//                break;

//            case Game::thinkerSeqStart+14:
//                // Закрыть сапёра
//                delete game->voltorbgame;
//                game->music->setCurrentIndex(2);
//                game->current_music->setVolume(50);
//                game->current_music->play();
//                game->scene->setSceneRect(0,0,Game::worldSize,Game::worldSize);
//                game->setSceneRect(game->currentViewPos.x(),game->currentViewPos.y(),800,600);
//                game->progress = Game::PHILOPHOBE_QUEST_COMPLETE;
//                game->save();
//                getBox(Game::thinkerSeqStart+15,Game::thinkerSeqStart+18);
//                break;

//            case Game::deadmanSeq7Start+25:
//                game->deadman->setParentItem(0);
//                game->deadman->setPos(game->player->x()+game->player->boundingRect().width()+10,game->player->y());
//                game->exit->show();
//                break;

//            case Game::witchSeqStart+1:
//                game->witch->show();
//                getBox(Game::witchSeqStart+2,Game::witchSeqStart+6);
//                break;

//            case Game::witchSeqStart+6:
//                game->music->setCurrentIndex(8);
//                game->current_music->setVolume(100);
//                game->current_music->play();
//                game->witch->start();
//                break;

//            case Game::witchSeqStart+10:
//                game->music->setCurrentIndex(9);
//                game->current_music->setVolume(100);
//                game->current_music->play();
//                getBox(Game::witchSeqStart+11,Game::witchSeqStart+25);
//                break;

//            case Game::witchSeqStart+25:
//                game->setBackgroundBrush(QBrush(QImage(":/images/cave-bg-bright.png")));
//                game->progress = Game::DEADMAN_REVIVED;
//                break;

//            case Game::deadmanSeq8Start+16:
//                game->progress = Game::DEADMANS_FAREWELL;
//                game->save();
//                game->deadman->setParentItem(game->player);
//                game->deadman->setPos(game->player->boundingRect().width(),0);
//                break;
//            }


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

    if (game->scene->width() > 800) {
        xPos += game->player->x() + game->player->boundingRect().width()*game->player->scale() / 2 - game->sceneRect().width() / 2;
        yPos += game->player->y() + game->player->boundingRect().height()*game->player->scale() / 2 - game->sceneRect().height() / 2 +5;
        setRect(xPos,yPos,800,150);
    }

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

//void DialogBox::monologueCall() {
//    game->progress = Game::OUTSIDE_EMPTINESS_DISCOVERED;
//    getBox(Game::deadmanSeq7Start+26,Game::deadmanSeq7Start+27);
//    delete game->timer;
//}
