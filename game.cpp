#include "game.h"
#include "button.h"
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QDebug>

Game::Game(QWidget* parent) {
    Q_UNUSED(parent);
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,800,600);

    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(scene->width(),scene->height());

    music = new QMediaPlaylist();
    music->addMedia(QUrl("qrc:/sounds/Been Thinking About It.mp3"));
    music->addMedia(QUrl("qrc:/sounds/Waking up in the Morning.mp3"));
    music->addMedia(QUrl("qrc:/sounds/A Step Forward Into Terror.mp3"));
    music->addMedia(QUrl("qrc:/sounds/Misato.mp3"));
    music->addMedia(QUrl("qrc:/sounds/Decisive Battle.mp3"));
    music->addMedia(QUrl("qrc:/sounds/A Moment When Tension Breaks.mp3"));
    music->addMedia(QUrl("qrc:/sounds/When I Find Peace of Mind.mp3"));
    music->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    current_music = new QMediaPlayer();
    current_music->setPlaylist(music);
}

void Game::displayMainMenu() {
    music->setCurrentIndex(0);
    current_music->setVolume(100);
    current_music->play();

    QFile save("save.txt");
    if (save.open(QIODevice::ReadOnly)) {
        QTextStream in(&save);
        QString str;
        in >> str;
        int in_progress = str.toInt();
        save.close();

        if (in_progress >= 0 && in_progress < 20)  {
            progress = static_cast<Progress>(in_progress);
        }
    }

    scene->clear();
    setBackgroundBrush(QBrush(QImage(":/images/bg.jpg")));

    QGraphicsTextItem* titleText = new QGraphicsTextItem(QString("Клевер"));
    QFont titleFont("Pushkin",70);
    titleText->setFont(titleFont);
    int txPos = width()/2 - titleText->boundingRect().width()/2;
    int tyPos = 120;
    titleText->setPos(txPos,tyPos);
    scene->addItem(titleText);

    Button* playButton;
    if (progress != CLOVER_QUEST_COMPLETE) {
        playButton = new Button(QString("Играть"));
    } else {
        playButton = new Button(QString("Заново"));
    }
    int bxPos = width()/2 - playButton->boundingRect().width()/2;
    int byPos = 275;
    playButton->setPos(bxPos,byPos);
    connect(playButton,SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(playButton);

    Button* quitButton = new Button(QString("Выйти"));
    int qxPos = width()/2 - quitButton->boundingRect().width()/2;
    int qyPos = 350;
    quitButton->setPos(qxPos,qyPos);
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(quitButton);

}

void Game::init() {
    scene->clear();
    scene->setSceneRect(0,0,800,600);
    setSceneRect(0,0,800,600);

    dialogbox = new DialogBox();
    dialogbox->setFlag(QGraphicsItem::ItemIsFocusable);
    scene->addItem(dialogbox);

    player = new Player();
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    int pxPos = 0;
    int pyPos = height()/2 - player->boundingRect().height()*player->scale()/2;
    player->setPos(pxPos,pyPos);
    scene->addItem(player);
    connect(player,SIGNAL(dialogCall(int, int)),dialogbox,SLOT(getBox(int, int)));

    passage = new Passage();
    connect(passage,SIGNAL(game_advance()),this,SLOT(outside()));
    scene->addItem(passage);
}

void Game::start(){
    if (progress == CLOVER_QUEST_COMPLETE) {
        progress = START;
    }
    init();
    setBackgroundBrush(QBrush(QImage(":/images/bg-start.png")));

    player->setPos(scene->width()/2-player->boundingRect().width()/2, scene->height()/2-player->boundingRect().height()/2);

    gardener = new Gardener();
    gardener->setPos(scene->width()*2/5-gardener->boundingRect().width()/2, scene->height()/2-gardener->boundingRect().height()/2);
    scene->addItem(gardener);

    player->set_direction(Player::directions::LEFT);

    switch (progress) {
    case START:
        setBackgroundBrush(QBrush(Qt::black));
        passage->hide();
        gardener->hide();
        current_music->stop();
        dialogbox->getBox(gardenerSeqStart,gardenerSeqStart+4);
        break;
    case ENERGY_DRINKS_GAME_COMPLETE: case BUGS_DEFEATED:
        setBackgroundBrush(QBrush(QImage(":/images/bg-start.png")));
        passage->setPixmap(QPixmap(":/images/passage-1.png"));
        break;
    default:
        setBackgroundBrush(QBrush(QImage(":/images/bg-start.png")));
        break;
    }
    show();
}

void Game::outside() {
    init();
    switch (progress) {
    case INTRO_COMPLETE: case PHONES_GAME_STARTED:
        phones_game();
        break;
    case PHONES_GAME_COMPLETE:
        mice_meeting();
        break;
    case ENERGY_DRINKS_GAME_COMPLETE:
        bug_fight();
        break;
    case BUGS_DEFEATED:
        clover_search();
        break;
    case CLOVER_QUEST_COMPLETE:
        displayMainMenu();
        break;
    default:
        break;
    }
}

void Game::phones_game() {
    QString str = ":/images/bg-buzz-%1.png";
    setBackgroundBrush(QBrush(QImage(str.arg(Buzz::current_level()))));

    if (Buzz::current_level() == 1) {
        music->setCurrentIndex(2);
        current_music->setVolume(50);
        current_music->play();
    } else if (Buzz::current_level() == 3) {
        int pxPos = scene->width()/2 - player->boundingRect().width()*player->scale()/2;
        int pyPos = 0;
        player->setPos(pxPos,pyPos);
    }

    Buzz * buzz[3];
    for (int i = 0; i < 3; ++i) {
        if (i == 2 && Buzz::current_level() < 3) break;
        buzz[i] = new Buzz(i);
        scene->addItem(buzz[i]);
    }
    if (Buzz::current_level() > 1) {
        dialogbox->getBox(phonesSeqStart+Buzz::current_level(), phonesSeqStart+Buzz::current_level());
    }

    show();
}

void Game::mice_meeting() {
    setBackgroundBrush(QBrush(QImage(":/images/bg-buzz-1.png")));

    music->setCurrentIndex(3);
    current_music->setVolume(50);
    current_music->play();

    Mice * mice = new Mice();
    scene->addItem(mice);

    dialogbox->getBox(preMiceSeqStart, preMiceSeqStart+3);
    passage->hide();
}

void Game::drink_game() {
    drinkgame = new DrinkGame();
    connect(drinkgame,SIGNAL(result(int, int)),dialogbox,SLOT(getBox(int, int)));
    scene->addItem(drinkgame);
    show();
}

void Game::bug_fight() {
    setBackgroundBrush(QBrush(QImage(":/images/bg-fight.png")));

    music->setCurrentIndex(4);
    current_music->setVolume(50);
    current_music->play();

    for (int i = 0; i < 5; ++i) {
        bug[i] = new Bug();
        bug[i]->setPos(rand()%500+300, rand()%400);
        scene->addItem(bug[i]);
    }

    passage->hide();

    dialogbox->getBox(bugFightStart,bugFightStart+4);
    show();
}

void Game::clover_search() {
    setBackgroundBrush(QBrush(QImage(":/images/bg-field-fence.png")));

    music->setCurrentIndex(5);
    current_music->setVolume(50);
    current_music->play();

    Flower * flower[14];
    for (int i = 0; i < 14; ++i) {
        flower[i] = new Flower(i);
        scene->addItem(flower[i]);
        if (i > 0) connect(flower[i-1],SIGNAL(show_next()),flower[i],SLOT(appear()));
        if (i == 13) clover = flower[i];
    }

    passage->hide();

    dialogbox->getBox(cloverSeqStart,cloverSeqStart+4);
    show();
}

void Game::clover_game() {
    clovergame = new CloverGame();
    connect(clovergame,SIGNAL(result(int, int)),dialogbox,SLOT(getBox(int, int)));
    scene->addItem(clovergame);
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

const QVector<Speechline> Game::speech = {
    {":/images/player-sad.png", "Я ненавижу этот день..."}, // 0
    {":/images/player-sad.png", "Почему всё так складывается? Что со мной?"},
    {":/images/player-sad.png", "Почему так темно?"},
    {":/images/gardener1.png", "Кто это здесь?"},
    {":/images/gardener1.png", "Проснись!"},
    {":/images/player.png", "А? Где это я? И кто ты?"}, // +5
    {":/images/gardener.png", "Я пришёл поработать немного у себя в саду, но нашёл здесь тебя спящей. Ты в порядке? Как ты здесь оказалась?"},
    {":/images/player.png", "Я не знаю..."},
    {":/images/player-sad.png", "В последнее время у меня всё идёт наперекосяк,<br>и вот теперь это..."},
    {":/images/gardener.png", "Всё наперекосяк, говоришь?.."},
    {":/images/player-sad.png", "Как будто не везёт постоянно. Я так устала от всего этого..."},
    {":/images/gardener.png", "Может быть, тебе просто нужно немного удачи?"},
    {":/images/player-annoyed.png", "Не помешало бы. Но вообще, как мне попасть домой?"},
    {":/images/gardener.png", "Можно сказать, в этот раз тебе всё-таки повезло.<br>Жёлтая дорожка ведёт к выходу, он довольно недалеко, а по пути ты можешь найти кое-что, что должно тебе помочь."},
    {":/images/player.png", "Что-то, что должно мне помочь? В каком смысле?"},
    {":/images/gardener.png", "Там будет небольшая клумба, где растут клеверы.<br>Если тебе повезёт, то ты сможешь найти четырёхлистник,<br>а они приносят удачу."},
    {":/images/player.png", "Они же очень редкие, возможно, что такого там даже не будет. Почему ты думаешь, что я его найду?"},
    {":/images/gardener.png", "Ну, в конце концов, это волшебный сад. Я думаю, если ты здесь появилась, то обязательно найдёшь свой четырёхлистный клевер."},
    {":/images/player-annoyed.png", "<i>Да уж, чтобы клеверы на клумбе росли...<br>Точно волшебный сад...</i>"},
    {":/images/gardener-think.png", "Ну, это пока просто хобби, и я не так много времени ему уделяю, как хотел бы..."},
    {":/images/gardener-shocked.png", "Постой. Есть одна сложность, я забыл тебе сказать."},
    {":/images/player.png", "Какая?"},
    {":/images/gardener.png", "Как ты относишься к насекомым?"},
    {":/images/player-fear.png", "Ужасно! Я панически их боюсь!"},
    {":/images/gardener-sad.png", "Беда... Я давно здесь не был, и их тут развелось довольно много... Придётся идти мимо них."},
    {":/images/player-sad.png", "А другого пути нет?"},
    {":/images/gardener-think.png", "Это самый короткий. Не бойся, что-нибудь придумаем.<br>Они не должны тебя трогать, но жужжат громко и могут сбить с толку. С учётом того, что ты их не переносишь, пройти будет сложно..."},
    {":/images/gardener.png", "Я придумал! Вот, возьми наушники, чтобы их жужжание тебе не мешало. Заодно я смогу связываться с тобой по пути, очень удобно."},
    {":/images/player.png", "Хорошо. Постой..."},
    {":/images/player-annoyed.png", "Это же строительные..."},
    {":/images/gardener-smug.png", "Да, защищают от шума лучше всего."},
    {":/images/player.png", "Как ты через них связываться собрался?"},
    {":/images/gardener-think.png", "..."},
    {":/images/gardener.png", "Ну, в любом случае стоит попробовать."},
    {":/images/gardener.png", "Запомни: когда видишь, что звуковая волна приближается<br>к тебе, нажми ПРОБЕЛ и наушники тебя защитят. Если не успеешь, то не сможешь пройти дальше."},
    {":/images/gardener.png", "Если готова, то попробуй пройти дальше по дороге,<br>посмотри, что там."}, //+35

    {":/images/player.png", "Значит, наушники можно надевать пробелом... Попробуем."}, // phones
    {":/images/gardener.png", "Удачи!"},
    {":/images/gardener.png", "Молодец, отлично справляешься!"},
    {":/images/gardener.png", "Кажется, здесь немного посложнее, но у тебя получится."},

    {":/images/gardener.png", "Отлично, похоже, ты прошла."}, // pre-Mice
    {":/images/player.png", "Наконец-то жуки закончились. Я уже немного устала..."},
    {":/images/player.png", "Стой, что это за странная компания тут?"},
    {":/images/player.png", "Наверно, надо уточнить у них дорогу."}, // +3

    {":/images/mouse-3.png", "Я тебе говорю, этот лучше!"}, // mice
    {":/images/mouse-4.png", "Ты чего, он же вообще отстой!"},
    {":/images/player.png", "Привет, вы не знаете, тут можно по дороге пройти?<br>Впереди нет жуков?"},
    {":/images/mouse-1.png", "А? Там самое их гнездо. Просто так не пройдёшь."},
    {":/images/mouse-2.png", "Если тебе нужно вперёд, придётся их победить."},
    {":/images/player.png", "Победить? У меня совсем нет сил, чтобы драться...<br>Тем более, с этими жуками..."},
    {":/images/mouse-1.png", "Если нет сил, значит, просто нужно зарядиться!"},
    {":/images/mouse-3.png", "У нас как раз 5 банок, так что одной можем поделиться."},
    {":/images/player.png", "Вы о чём?"},
    {":/images/mouse-2.png", "Мы долго спорим, какой из этих энергосов вкуснее,<br>и никак не можем прийти к общему решению."},
    {":/images/mouse-4.png", "Попробуй их и рассуди нас, а мы отдадим тебе лучший."},
    {":/images/mouse-1.png", "Тогда и жуков легко одолеешь."},
    {":/images/player.png", "Пробовать энергосы? Это я умею, давайте!"}, // +12

    {":/images/player.png", "Вроде вкусно, но надо сравнить..."}, // energy
    {":/images/player-sad.png", "Самое отвратительное из энергетиков<br>в моей жизни сквозь года."}, // разница вкусов -4
    {":/images/player-annoyed.png", "По сравнению с тем – отстой полный!"},
    {":/images/player.png", "Пить можно, но не нужно.<br>Предыдущий мне понравился больше."},
    {":/images/player.png", "Как предыдущий, только с менее насыщенным вкусом.<br>В целом хорошо."},
    {":/images/player.png", "Вкус такой же. Надо попробовать другие."},   // разница вкусов 0
    {":/images/player.png", "Сладко, но достаточно вкусненько.<br>Этот мне нравится немного больше."},
    {":/images/player.png", "На вкус как тот, только меньше сахара. Огонь!"},
    {":/images/player-smile.png", "Что-то похожее по вкусу на предыдущий, НО ЛУЧШЕ.<br>И очень газированно!"},
    {":/images/player-smile.png", "Ребята, ребята, это не учебная тревога!<br>Найден невероятно вкусный энергетик!"},
    {":/images/mouse-2.png", "Похоже, ты ещё не расположила все энергосы по местам."}, //
    {":/images/mouse-4.png", "На каждом месте должно быть не больше одной банки."},
    {":/images/mouse-3.png", "Распробуй энергосы получше, пока кажется, что что-то не совсем правильно."}, //
    {":/images/mouse-1.png", "Всё правильно!"}, //
    {":/images/mouse-1.png", "Спасибо за помощь! Возьми баночку в награду и выпей, чтобы набраться сил."},
    {":/images/player.png", "С учётом того, сколько пришлось перепробовать, силы уже начинают ощущаться. Хотя, сейчас не помешает ещё."}, //+15

    {":/images/bug.png", "Сдавайся! Ты не пройдёшь дальше!"}, // bugs
    {":/images/gardener-think.png", "Похоже, одними наушниками уже не обойдёшься.<br>В этот раз попробуй стрелять в жуков с помощью пробела."},
    {":/images/gardener.png", "Энергии теперь тебе должно хватить."},
    {":/images/player-annoyed.png", "Я и правда чувствую прилив сил, но стрелять? Чем?"},
    {":/images/player.png", "Видимо, придётся просто попробовать. Пробел, да?"},

    {":/images/boss.png", "Ты победила моих слуг, но я не позволю тебе пройти дальше."}, //+5
    {":/images/player.png", "Заюш, ты лучше уйди, а то и тебя застрелю."},
    {":/images/boss.png", "Убирайся! Это моя территория!"}, // +7
    {":/images/boss.png", "Похоже, надо искать другое место..."},
    {":/images/player-smile.png", "Фух... Я в самом деле победила!"},
    {":/images/gardener.png", "Отличная работа! Теперь вперёд, клумба прямо перед тобой."},

    {":/images/player-annoyed.png", "Вот это клумба... Целый один цветочек."},
    {":/images/player-annoyed.png", "Совсем ты за своим садом не ухаживал. Не то что четырёхлистника, даже обычных клеверов не видно."},
    {":/images/player-annoyed.png", "И как тут можно искать что-то?"},
    {":/images/gardener.png", "Не торопи события. Почему бы тебе для начала не посмотреть поближе на этот цветок?"},
    {":/images/player.png", "Что ж, ну ладно. Больше тут и не на что смотреть."},

    {":/images/player.png", "Тут цифра выпала. Что это значит?"},
    {":/images/gardener.png", "Возможно, цветы хотят что-то тебе сказать. Видишь следующий?"},
    {":/images/player.png", "Да, попробую проверить его тоже."},

    {":/images/player.png", "Сплошные нули. Так и должно быть?"},
    {":/images/player-annoyed.png", "И этот тоже..."},
    {":/images/player.png", "Наконец-то что-то больше нуля! Интересно, что это за цифры?"},
    {":/images/player.png", "Какая-то извилистая дорога получается.<br>Любопытно, куда эти цветы меня ведут?"},
    {":/images/player.png", "Это что? Неужели?.."},
    {":/images/player.png", "Четырёхлистный клевер..."},
    {":/images/player-smile.png", "Вот оно что...<br>Только что всё-таки означают цифры?"},
    {":/images/gardener.png", "Попробуй записать их в том порядке, в котором ты их проходила.<br>Кто знает, вдруг это пригодится."},
    {":/images/player-smile.png", "Ладно... Глядя на свой путь, наверно, уже не запутаюсь.<br>Может быть, сделать скриншот на всякий случай тоже будет полезно."},
    {":/images/gardener.png", "Ты уже рассмотрела клевер? Приглядись к нему тоже."},
    {":/images/player.png", "Сейчас, посмотрю..."},

    {":/images/player.png", "Что это? У него на листьях тоже что-то есть!"},
    {":/images/gardener.png", "У клевера есть свои секреты. Говорят, он приносит удачу, но вполне возможно, что эта удача может быть также и в виде некого талисмана."},
    {":/images/gardener.png", "Если ты разгадаешь его загадку, то обязательно случится что-то интересное."},
    {":/images/gardener.png", "Молодец, у тебя получилось! Запомни эти цифры, начиная от стебелька и по часовой стрелке. Это должно быть очень важно."},

    {":/images/gardener.png", "Теперь ты наконец можешь вернуться домой, выход впереди."},
    {":/images/player.png", "А как же все эти цифры? Что в итоге делать с ними?"},
    {":/images/gardener.png", "Я уверен, что если ты всё записала верно, то они пригодятся тебе впереди."},
    {":/images/gardener.png", "К примеру, ты когда-нибудь проходила какие-либо странные спонтанные квесты?"},
    {":/images/player.png", "Странные квесты?.. Было дело, но это довольно давно..."},
    {":/images/gardener.png", "Я практически уверен, что можно попробовать проверить эти цифры в том месте, где ты была в прошлый раз."},
    {":/images/player-smile.png", "Хорошо... Я постараюсь проверить."},
    {":/images/gardener.png", "Пока ты не ушла, не забудь взять и сам клевер с собой. И, кстати, с днём рождения) Будем считать, что это мой маленький подарок."},
    {":/images/gardener.png", "Знаю, что глупо советовать не грустить, но надеюсь, что он хоть немного поможет тебе в этом."},
    {":/images/player-smile.png", "Спасибо... Это очень..."},
    {":/images/gardener.png", "Что говоришь? Связь слабеет, и я уже не совсем хорошо тебя слышу. Можешь повторить?"},
    {":/images/player-smile.png", "Я говорю, надеюсь, мы ещё увидимся."},
    {":/images/gardener.png", "Обязательно!"},
    {":/images/player-smile.png", "Тогда я пойду, у меня ведь есть свои дела сегодня.<br>До встречи, садовник."},
    {":/images/gardener.png", "Удачи тебе!"},

    {":/images/gardener.png", "Кстати, я умею сохранять игру, так что можешь не беспокоиться, что много потеряешь, если нужно будет прерваться."},
    {":/images/gardener.png", "Я загрузил сохранённую игру, так что можешь смело идти вперёд."},
};
