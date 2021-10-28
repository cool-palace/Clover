#include "phonesgame.h"
extern Game * game;

PhonesGame::PhonesGame(QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/images/bg.png"));

    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(255,174,201,100));

//    cond_bg = new QGraphicsRectItem(this);
//    cond_bg->setRect(50,30, 700, 100);
//    cond_bg->setBrush(brush);

//    conditions = new QGraphicsTextItem(this); //
//    QString cond = "Чтобы танцевать, нажимай на стрелки на клавиатуре в такт ритму, подсвечивающиеся подсказки указывают правильное направление.<br>Для полного танца нужно набрать 16 очков.";
//    QString str1 = "<p style=\"text-align:center;\">%1</p>";
//    conditions->setHtml(str1.arg(cond));
//    conditions->setFont({"Comic Sans", 14});
//    conditions->setTextWidth(700);
//    conditions->setPos(50,40);

    player = new Player(this);
    int pxPos = Buzz::current_level() < 3 ? 0 : game->scene->width()/2 - player->boundingRect().width()*player->scale()/2;
    int pyPos = Buzz::current_level() < 3 ? game->scene->height()/2 - player->boundingRect().height()*player->scale()/2 : 0;
    player->setPos(pxPos,pyPos);

    buzz = new QGraphicsEllipseItem(this);
    int radius = 10;
    buzz->setRect(game->scene->width()/2 - radius, game->scene->height()/2 - radius, 2*radius, 2*radius);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(change()));
    timer->start(100);

    setEnabled(true);
    grabKeyboard();
}

PhonesGame::~PhonesGame() {
    delete player;
    delete buzz;
    delete timer;
}

void PhonesGame::change() {
    int diameter = buzz->rect().width() < 300 ? buzz->rect().width() + 10 : 20;
    int x = game->scene->width()/2 - diameter/2;
    int y = game->scene->height()/2 - diameter/2;
    buzz->setRect(x, y, diameter, diameter);
}

void PhonesGame::keyPressEvent(QKeyEvent * event) {

}
