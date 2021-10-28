#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include "player.h"
#include "npc.h"
#include "dialogbox.h"
#include "button.h"
//#include "phonesgame.h"
//#include "riddlebox.h"
#include <QVector>

class Game: public QGraphicsView{
    Q_OBJECT
public:
    // constructors
    Game(QWidget * parent=0);

    // public methods
    void save();

    // public attributes
    QGraphicsScene * scene;
    Player * player;
//    Gardener * gardener;
    DialogBox * dialogbox;
    QPointF currentViewPos = {0, 0};
//    Exit * exit;
//    Cave * cave;
//    Entrance * entrance;
//    Snake * snake[2];
//    Witch * witch;

    QTimer * timer;

    enum Progress {
        START,
        INTRO_COMPLETE,
        BUZZ_GAME_LEVEL_1,
        BUZZ_GAME_LEVEL_2,
        BUZZ_GAME_LEVEL_3,
        PHONES_QUEST_COMPLETE,
        ENERGY_DRINKS_QUEST_COMPLETE,
        FLYES_DEFEATED,
        CLOVER_QUEST_COMPLETE
    };

    Progress progress = START;

//    const static QVector<Riddle> riddles;

    const static QVector<Speechline> speech;

    constexpr static int worldSize = 2760;

    constexpr static int deadmanSeq1Start = 0;

public slots:
    void start();
    void outside();
    void phones_game();
    void displayMainMenu();
};

#endif // GAME_H
