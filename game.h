#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include "player.h"
#include "npc.h"
#include "dialogbox.h"
#include "button.h"
#include "drinkgame.h"
#include "clovergame.h"
#include <QVector>

class Game: public QGraphicsView{
    Q_OBJECT
public:
    // constructors
    Game(QWidget * parent = nullptr);

    // public methods
    void save();

    // public attributes
    QGraphicsScene * scene;
    Player * player;
    Gardener * gardener;
    Passage * passage;
    DialogBox * dialogbox;
    DrinkGame * drinkgame;
    CloverGame * clovergame;
    Flower* clover;
    Bug * bug[5];
    BugBoss * boss;
    QTimer * timer;
    QMediaPlaylist * music;
    QMediaPlayer * current_music;

    enum Progress {
        START,
        INTRO_COMPLETE,
        PHONES_GAME_STARTED,
        PHONES_GAME_COMPLETE,
        ENERGY_DRINKS_GAME_COMPLETE,
        BUGS_FIGHT_STARTED,
        BUGS_DEFEATED,
        CLOVER_QUEST_COMPLETE
    };

    Progress progress = START;

    const static QVector<Speechline> speech;

    constexpr static int worldSize = 2760;

    constexpr static int gardenerSeqStart = 0;
    constexpr static int phonesSeqStart = gardenerSeqStart + 36;
    constexpr static int preMiceSeqStart = phonesSeqStart + 4;
    constexpr static int miceSeqStart = preMiceSeqStart + 4;
    constexpr static int energySeqStart = miceSeqStart + 13;
    constexpr static int bugFightStart = energySeqStart + 16;
    constexpr static int cloverSeqStart = bugFightStart + 11;
    constexpr static int help = cloverSeqStart + 38;

public slots:
    void init();
    void start();
    void outside();

    void phones_game();
    void mice_meeting();
    void drink_game();
    void bug_fight();
    void clover_search();
    void clover_game();
    void displayMainMenu();
};

#endif // GAME_H
