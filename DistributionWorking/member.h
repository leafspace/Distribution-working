#ifndef MEMBER_H
#define MEMBER_H

#include <QString>

class OneDay
{
public:
    QString forenone_up;
    QString forenone_down;
    QString afternone_up;
    QString afternone_down;
    QString night;

    OneDay() {
        this->forenone_up = "";
        this->forenone_down = "";
        this->afternone_up = "";
        this->afternone_down = "";
        this->night = "";
    }

    QString getStr(int index);
};

class Member
{
private:
    QString username;
    OneDay OneWeek[7];

public:
    Member();
    Member(QString username);

    void setUsername(QString username);
    QString getUsername();
    void setDay(int day, OneDay oneDay);
    OneDay getDay(int day);
};

#endif // MEMBER_H
