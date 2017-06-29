#include "member.h"

QString OneDay::getStr(int index)
{
    switch(index)
    {
    case 0: return this->forenone_up;    break;
    case 1: return this->forenone_down;  break;
    case 2: return this->afternone_up;   break;
    case 3: return this->afternone_down; break;
    case 4: return this->night;          break;
    }
    return "";
}

Member::Member()
{
    this->username = "";
}

Member::Member(QString username)
{
    this->username = username;
}

void Member::setUsername(QString username)
{
    this->username = username;
}

QString Member::getUsername()
{
    return this->username;
}

void Member::setDay(int day, OneDay oneDay)
{
    //this->OneWeek[day] = oneDay;
    this->OneWeek[day].forenone_up = oneDay.forenone_up;
    this->OneWeek[day].forenone_down = oneDay.forenone_down;
    this->OneWeek[day].afternone_up = oneDay.afternone_up;
    this->OneWeek[day].afternone_down = oneDay.afternone_down;
    this->OneWeek[day].night = oneDay.night;
}

OneDay Member::getDay(int day)
{
    return this->OneWeek[day];
}
