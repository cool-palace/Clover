#include "game.h"
#include "button.h"
#include <QKeyEvent>
#include <QGraphicsTextItem>

Game::Game(QWidget* parent)
{
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,800,600); // make the scene 800x600 instead of infinity by infinity (default)

    // make the newly created scene the scene to visualize (since Game is a QGraphicsView Widget,
    // it can be used to visualize scenes)
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(scene->width(),scene->height());

//    music = new QMediaPlaylist();
//    music->addMedia(QUrl("qrc:/sounds/title_screen.mp3"));
//    music->addMedia(QUrl("qrc:/sounds/cave_theme.mp3"));
//    music->addMedia(QUrl("qrc:/sounds/outside_theme.mp3"));
//    music->addMedia(QUrl("qrc:/sounds/solving_riddles.mp3"));
//    music->addMedia(QUrl("qrc:/sounds/minigame.mp3"));
//    music->addMedia(QUrl("qrc:/sounds/snake_fight.mp3"));
//    music->addMedia(QUrl("qrc:/sounds/sunken_heart.mp3"));
//    music->addMedia(QUrl("qrc:/sounds/empty_world.mp3"));
//    music->addMedia(QUrl("qrc:/sounds/witch_fight.mp3"));
//    music->addMedia(QUrl("qrc:/sounds/deadman_revived.mp3"));
//    music->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

//    current_music = new QMediaPlayer();
//    current_music->setPlaylist(music);
}

void Game::start(){

//    if (progress == DEADMANS_FAREWELL) {
//        progress = START;
//    }

//    if (progress == START) {
//        current_music->stop();
//    } else {
//        music->setCurrentIndex(1);
//        current_music->setVolume(50);
//        current_music->play();
//    }

    // clear the screen
    scene->clear();
    setBackgroundBrush(QBrush(QImage(":/images/cave-bg.png")));
    scene->setSceneRect(0,0,800,600);
    setSceneRect(0,0,800,600);
    currentViewPos = {0, 0};

    // create the dead man
//    deadman = new Deadman();
//    int dxPos = scene->width()/2 - deadman->boundingRect().width()/2*deadman->scale();
//    int dyPos = 100;
//    deadman->setPos(dxPos, dyPos);
//    scene->addItem(deadman);

    // create a dialog box
    dialogbox = new DialogBox();
    dialogbox->setFlag(QGraphicsItem::ItemIsFocusable);
    scene->addItem(dialogbox);

    // create an exit
//    exit = new Exit();
//    int exPos = scene->width()/2 - exit->boundingRect().width()/2;
//    int eyPos = scene->height() - exit->boundingRect().height();
//    exit->setPos(exPos, eyPos);
//    if (progress == START || progress == OUTSIDE_EMPTINESS_DISCOVERED) {
//        exit->hide();
//    } else exit->show();
//    scene->addItem(exit);

    // create the player
    player = new Player();
    int pxPos = scene->width()/2 - player->boundingRect().width()*player->scale()/2;
    int pyPos;
    if (progress == START) {
        pyPos = scene->height()/2 - player->boundingRect().height()*player->scale()/2;
    } else pyPos = 450;
    player->setPos(pxPos,pyPos);
    // make the player focusable and set it to be the current focus
    player->setMovable();
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    scene->addItem(player);

    connect(player,SIGNAL(dialogCall(int, int)),dialogbox,SLOT(getBox(int, int)));
    connect(player,SIGNAL(goingOut()),this,SLOT(outside()));

    show();
}

void Game::displayMainMenu(){

//    music->setCurrentIndex(0);
//    current_music->setVolume(100);
//    current_music->play();

//    QFile save("save.txt");
//    if (save.open(QIODevice::ReadOnly)) {
//        QTextStream in(&save);
//        QString str;
//        in >> str;
//        int in_progress = str.toInt();
//        save.close();

//        if (in_progress >= 0 && in_progress < 20)  {
//            progress = static_cast<Progress>(in_progress);
//        }
//    }

    scene->clear();
    setBackgroundBrush(QBrush(QImage(":/images/bg.jpg")));
    // create the title text
    QGraphicsTextItem* titleText = new QGraphicsTextItem(QString("Клевер"));
    QFont titleFont("Pushkin",70);
    titleText->setFont(titleFont);
    int txPos = this->width()/2 - titleText->boundingRect().width()/2;
    int tyPos = 120;
    titleText->setPos(txPos,tyPos);
    scene->addItem(titleText);

    // create the play button

//    if (progress != DEADMANS_FAREWELL) {
        Button* playButton = new Button(QString("Читать"));
//    } else {
//        playButton = new Button(QString("Заново"));
//    }
    int bxPos = this->width()/2 - playButton->boundingRect().width()/2;
    int byPos = 275;
    playButton->setPos(bxPos,byPos);
//    connect(playButton,SIGNAL(clicked()),this,SLOT(start()));
//    connect(playButton,SIGNAL(clicked()),this,SLOT(phones_game()));
    connect(playButton,SIGNAL(clicked()),this,SLOT(bug_fight()));
//    connect(playButton,SIGNAL(clicked()),this,SLOT(drink_game()));
    scene->addItem(playButton);

    // create the quit button
    Button* quitButton = new Button(QString("Выйти"));
    int qxPos = this->width()/2 - quitButton->boundingRect().width()/2;
    int qyPos = 350;
    quitButton->setPos(qxPos,qyPos);
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(quitButton);

//    if (progress == DEADMANS_FAREWELL) {
//        lastButton = new Button(QString("Клад"));
//        int lxPos = this->width()/2 - quitButton->boundingRect().width()/2;
//        int lyPos = 425;
//        lastButton->setPos(lxPos,lyPos);
//        connect(lastButton,SIGNAL(clicked()),this,SLOT(deadmans_note()));
//        scene->addItem(lastButton);
//    }
}

void Game::outside() {

//    if (progress != FIFTH_RIDDLE_SOLVED) {
//        music->setCurrentIndex(2);
//        current_music->play();
//    } else {
//        music->setCurrentIndex(7);
//        current_music->setVolume(50);
//        current_music->play();
//    }

    // clear the screen
    scene->clear();
    setBackgroundBrush(QBrush(QImage(":/images/bg-big.png")));
    scene->setSceneRect(0,0,worldSize,worldSize);
    setSceneRect(980,0,800,600);
    currentViewPos = {980, 0};

    // create the cave
//    cave = new Cave();
//    int cxPos = scene->width()/2 - cave->boundingRect().width()*cave->scale()/2;
//    int cyPos = 0;
//    cave->setPos(cxPos, cyPos);
//    entrance = new Entrance(cave);
//    entrance->setPos(127,195);
//    scene->addItem(cave);

    // create a dialog box
    dialogbox = new DialogBox();
    dialogbox->setFlag(QGraphicsItem::ItemIsFocusable);
    scene->addItem(dialogbox);

    // create the player
    player = new Player();
    int pxPos = scene->width()/2 - player->boundingRect().width()*player->scale()/2;
    int pyPos = height()/2 - player->boundingRect().height()*player->scale()/2;
    player->setPos(pxPos,pyPos);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    scene->addItem(player);
    connect(player,SIGNAL(dialogCall(int, int)),dialogbox,SLOT(getBox(int, int)));
    connect(player,SIGNAL(goingIn()),this,SLOT(start()));

    show();
}

void Game::phones_game() {
    scene->clear();
    QString str = ":/images/bg-buzz-%1.png";
    setBackgroundBrush(QBrush(QImage(str.arg(Buzz::current_level()))));
    scene->setSceneRect(0,0,800,600);
    setSceneRect(0,0,800,600);

//    music->setCurrentIndex(4);
//    current_music->setVolume(10);
//    current_music->play();

    // create a dialog box
    dialogbox = new DialogBox();
    dialogbox->setFlag(QGraphicsItem::ItemIsFocusable);
    scene->addItem(dialogbox);

    player = new Player();
    int pxPos = Buzz::current_level() < 3 ? 0 : scene->width()/2 - player->boundingRect().width()*player->scale()/2;
    int pyPos = Buzz::current_level() < 3 ? height()/2 - player->boundingRect().height()*player->scale()/2 : 0;
    player->setPos(pxPos,pyPos);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    scene->addItem(player);
    connect(player,SIGNAL(dialogCall(int, int)),dialogbox,SLOT(getBox(int, int)));

    Buzz * buzz[3];
    for (int i = 0; i < 3; ++i) {
        if (i == 2 && Buzz::current_level() < 3) break;
        buzz[i] = new Buzz(i);
        scene->addItem(buzz[i]);
    }

    Passage * passage = new Passage(Buzz::current_level());
    scene->addItem(passage);
    connect(passage,SIGNAL(phone_game_advance()),this,SLOT(phones_game()));
    connect(passage,SIGNAL(phone_game_end()),this,SLOT(displayMainMenu()));
//    buzz->setPos(scene->width()/2, scene->height()/2);
//    connect(phonesgame,SIGNAL(result(int, int)),dialogbox,SLOT(getBox(int, int)));
    show();
}

void Game::drink_game() {
//    scene->setSceneRect(0,0,800,600);
//    setSceneRect(0,0,800,600);

//    music->setCurrentIndex(4);
//    current_music->setVolume(10);
//    current_music->play();

    // create a dialog box
    dialogbox = new DialogBox();
    dialogbox->setFlag(QGraphicsItem::ItemIsFocusable);

    DrinkGame * drinkgame = new DrinkGame();
    scene->addItem(drinkgame);
    scene->addItem(dialogbox);

    connect(drinkgame,SIGNAL(result(int, int)),dialogbox,SLOT(getBox(int, int)));
    show();
}

void Game::bug_fight() {
//    scene->setSceneRect(0,0,800,600);
//    setSceneRect(0,0,800,600);
    scene->clear();
    setBackgroundBrush(QBrush(QImage(":/images/bg-fight.png")));

//    music->setCurrentIndex(4);
//    current_music->setVolume(10);
//    current_music->play();

    // create a dialog box
    dialogbox = new DialogBox();
    dialogbox->setFlag(QGraphicsItem::ItemIsFocusable);
    scene->addItem(dialogbox);

    player = new Player();
    player->setPixmap(QPixmap(":/images/player-right.png"));
    int pxPos = 0;
    int pyPos = height()/2 - player->boundingRect().height()*player->scale()/2;
    player->setPos(pxPos,pyPos);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    scene->addItem(player);
    connect(player,SIGNAL(dialogCall(int, int)),dialogbox,SLOT(getBox(int, int)));

    for (int i = 0; i < 5; ++i) {
        bug[i] = new Bug();
        bug[i]->setPos(rand()%500+300, rand()%500);
        scene->addItem(bug[i]);
    }

    dialogbox->getBox(bugFightStart,bugFightStart);
    show();
}

void Game::save() {
    QFile save("save.txt");
    if (save.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&save);
        out << progress;
        save.close();
    }
}

//const QVector<Riddle> Game::riddles = {
//    {"Две средние цифры года твоего рождения, повторённые дважды.", "0000"},
//    {"Средние две цифры — это последние цифры твоего предыдущего номера телефона, а цифры по краям их дублируют.", "4499"},
//    {"Первая цифра — номер дня недели,<br>в который ты получила эту открытку,<br>"
//      "а последние две — номер твоего дома.", "324"},
//    {"Для каждой буквы из своего имени<br>возьми их порядковые номера в алфавите и сложи их все вместе. В полученной сумме поменяй цифры местами.", "67"},
//    {"Две средние цифры из твоего почтового индекса и две последние цифры из номера твоей медкнижки.", "2515"}

//    };

const QVector<Speechline> Game::speech = {
    {":/images/player.png", "Где я?.."}, //0//
    {":/images/player.png", "Последнее, что я помню — это букет и та странная открытка.<br>Я открыла её, а дальше... Как я здесь оказалась?"},
    {":/images/player.png", "Здесь так темно и пусто... <br>Только чёрные стены и кучка земли..."},
    {":/images/player.png", "Надо осмотреться, может быть, получится найти выход."}, // +3
    {":/images/player-fear.png", "ААА!!! МЕРТВЕЦ!"}, // +4
    {":/images/deadman.png", "..."},
    {":/images/player-fear.png", "СПАСИТЕ!!!"},
    {":/images/deadman-uneasy.png", "Эм..."},
    {":/images/deadman-uneasy.png", "Успокойся, пожалуйста. Я тебя не съем."},
    {":/images/player-frown.png", "Ты кто?! Что это за место?! Как мне попасть домой?"},
    {":/images/deadman.png", "Это немного другой мир.<br>Если ты попала сюда, то вернуться домой будет сложно."},
    {":/images/player-frown.png", "Это ты меня здесь запер?!"},
    {":/images/deadman-uneasy.png", "Нет, ты чего... Я и сам заперт здесь.<br>Меня заколдовали давным-давно."},
    {":/images/player.png", "Заколдовали?"},
    {":/images/deadman.png", "Да... После этого я и стал выглядеть так...<br>Если не поторопиться, то ты тоже можешь превратиться здесь в кого-то."},
    {":/images/player.png", "В смысле, превратиться? В кого?"},
    {":/images/deadman.png", "Не знаю, как получится. В бабайку или что-то такое."},
    {":/images/player-fear.png", "И что мне теперь делать? Я не хочу быть бабайкой!<br>Я хочу домой!"},
    {":/images/deadman.png", "Я же говорю, это не так просто, иначе я бы сам здесь не сидел. Можно попробовать один способ, конечно, но придётся потрудиться. И я не совсем уверен, что сработает..."},
    {":/images/player-frown.png", "Ну же, расскажи скорее!"},
    {":/images/deadman.png", "Недавно я нашёл старую книгу с заклинаниями, и там было то, которое может нам помочь и открыть дверь обратно в настоящий мир."},
    {":/images/player-frown.png", "Так давай скорее его используем!"},
    {":/images/deadman.png", "Проблема в том, что в книге оно неполное. Чтобы закончить его, надо собрать <b>пять ключей</b>."},
    {":/images/player.png", "А где их найти? И что это за ключи вообще?"},
    {":/images/deadman.png", "Я знаю только то, они спрятаны в брикетах волшебного мела."},
    {":/images/player-annoyed.png", "Что за бред..."},
    {":/images/deadman-uneasy.png", "Звучит глупо, но так было написано в книге..."},
    {":/images/deadman.png", "Можешь мне не верить, но лучше попробовать их найти, пока есть время. Если ты превратишься в бабайку, пути назад не будет."},
    {":/images/player-annoyed.png", "<i>Ничего не поделаешь, придётся попытаться.</i>"},
    {":/images/player.png", "Ладно, но где мне искать эти ключи?"},
    {":/images/deadman.png", "Вот здесь, снизу, есть выход, попробуй посмотреть снаружи."}, //+30

    {":/images/bug.png", "Сдавайся!"}, //+30
    {":/images/player.png", "Я победила!"},

};
