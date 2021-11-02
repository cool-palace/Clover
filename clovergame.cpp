#include "clovergame.h"
#include "game.h"
#include <QDebug>
extern Game * game;

Digit::Digit(int digit, QGraphicsItem* parent) : QGraphicsTextItem(parent)
{
    if (digit < 0 || digit > 9) return;
    value_ = digit;

    QString str = "<p style=\"text-align:center;\"><i>%1</i></p>";
    setHtml(str.arg(value_));
    setFont({"Calibri", 40});
    setTextWidth(100);
    setPos(0,(120 - boundingRect().height())/2 + 5);

    setEnabled(false);
    setAcceptHoverEvents(false);
}

void Digit::increase() {
    if (value_ < 9) ++value_;
    else value_ = 0;
    QString str = "<p style=\"text-align:center;\"><i>%1</i></p>";
    setHtml(str.arg(value_));
}

void Digit::decrease() {
    if (value_ > 0) --value_;
    else value_ = 9;
    QString str = "<p style=\"text-align:center;\"><i>%1</i></p>";
    setHtml(str.arg(value_));
}

int Digit::value() {
    return value_;
}

CloverGame::CloverGame(QGraphicsItem * parent) : QObject(), QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/images/bg.jpg"));
    setZValue(3);
//    setPos(50, 10);

    clover = new QGraphicsPixmapItem(this);
    clover->setPixmap(QPixmap(":/images/clover.png"));
    clover->setPos(game->scene->width()/2-clover->boundingRect().width()/2, 133);

    correctSound = new QMediaPlayer();
    correctSound->setMedia(QUrl("qrc:/sounds/correct.wav"));

    wrongSound = new QMediaPlayer();
    wrongSound->setMedia(QUrl("qrc:/sounds/wrong.wav"));

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(255,174,201,150));

    QPen pen;
    QBrush pen_brush;
    pen_brush.setStyle(Qt::SolidPattern);
    pen_brush.setColor(QColor(255,255,255,150));
    pen.setBrush(pen_brush);

    cond_bg = new QGraphicsRectItem(this);
    cond_bg->setRect(50,30, 700, 100);
    cond_bg->setBrush(brush);
    cond_bg->setPen(pen);

    conditions = new QGraphicsTextItem(this);
    QString cond = "\"Зелёные листы\" – это количество верных цифр на правильных позициях.<br>\"Жёлтые листы\" – это количество цифр, находящихся не на своём месте.<br>Попробуй собрать четыре зелёных листа, чтобы разгадать секрет клевера.";
    QString str1 = "<p style=\"text-align:center;\">%1</p>";
    conditions->setHtml(str1.arg(cond));
    conditions->setFont({"Calibri", 14});
    conditions->setTextWidth(700);
    conditions->setPos(50,40);

    score_bg = new QGraphicsRectItem(this);
    score_bg->setRect(50,150, 170, 75);
    score_bg->setBrush(brush);
    score_bg->setPen(pen);

    score = new QGraphicsTextItem(this);
    score->setHtml(score_str.arg(0).arg(0));
    score->setFont({"Calibri", 14});
    score->setTextWidth(170);
    score->setPos(50,160);

    // memorizing the answer
    answer = "8680";

    digitsCount = answer.length();

    upButtons = new Button*[digitsCount];
    downButtons = new Button*[digitsCount];
    digits = new Digit*[digitsCount];


    qreal offset = (game->scene->width() - 100 * digitsCount - 20 * (digitsCount - 1)) / 2;
    QVector<QPointF> points = {{0*120+offset+70,330},
                               {1*120+offset,165},
                               {2*120+offset+30,185},
                               {2*120+offset+40,350}};
    QPointF down_offset = {0, 100};

    for (int i = 0; i < digitsCount; ++i) {

        upButtons[i] = new Button("Больше",this);
        upButtons[i]->setPos(points[i]);

        downButtons[i] = new Button("Меньше",this);
        downButtons[i]->setPos(points[i]+down_offset);

        digits[i] = new Digit(rand()%10,upButtons[i]);

        connect(upButtons[i],SIGNAL(clicked()),digits[i],SLOT(increase()));
        connect(downButtons[i],SIGNAL(clicked()),digits[i],SLOT(decrease()));
    }

    confirmButton = new Button("Готово",this);
    confirmButton->setPos(boundingRect().width()/2 - confirmButton->boundingRect().width()/2, 525);

    connect(confirmButton,SIGNAL(clicked()),this,SLOT(checkAnswer()));

    show();
}

CloverGame::~CloverGame() {
    delete[] upButtons;
    delete[] downButtons;
    delete[] digits;
    delete cond_bg;
    delete conditions;
    delete confirmButton;
    delete correctSound;
    delete wrongSound;
}

void CloverGame::checkAnswer() {
    QString guess;
    for (int i = 0; i < answer.size(); ++i) {
        QString s;
        s.setNum(digits[i]->value());
        guess += s;
    }

    QString used_guess = "nnnn";
    QString used_answer = "nnnn";
    int green = 0;
    int yellow = 0;
    // Поиск зелёных
    for (int i = 0; i < answer.size(); ++i) {
        if (guess[i] == answer[i]) {
            ++green;
            used_guess[i] = 'y';
            used_answer[i] = 'y';
        }
    }
    // Поиск жёлтых
    for (int i = 0; i < answer.size(); ++i) {
        for (int j = 0; j < answer.size(); ++j) {
            if (guess[i] == answer[j] && i != j
                    && used_guess[i] != 'y' && used_answer[j] != 'y') {
                ++yellow;
                used_guess[i] = 'y';
                used_answer[j] = 'y';
            }
        }
    }

    score->setHtml(score_str.arg(green).arg(yellow));

    if (green == 4) {
        emit result(Game::cloverSeqStart+22,Game::cloverSeqStart+22);
        correctSound->play();
    } else return;
}
