#ifndef SPEECHLINE_H
#define SPEECHLINE_H
#include <QString>

class Speechline
{
public:
    Speechline(QString in_speaker = ":/images/player.png", QString in_line = "hello");

    QString speaker;
    QString line;
};

#endif // SPEECHLINE_H
