#include "CycleInt.h"

CycleInt::CycleInt() :
    limit(CYCLE_LIMIT),
    number(0)
{
}

CycleInt::CycleInt(unsigned int lim) :
    limit(lim),
    number(0)
{
}

CycleInt::CycleInt(unsigned int lim, int num) :
    limit(lim),
    number(num)
{
    while(number >= limit) {
        number -= limit;
    }
}

CycleInt &CycleInt::operator=(const CycleInt &o)
{
    number = o.number;
    limit = o.limit;
    return *this;
}

CycleInt &CycleInt::operator=(const int &o)
{
    number = o;
    while(number >= limit) {
        number -= limit;
    }
    while(number < 0) {
        number += limit;
    }
    return *this;
}

CycleInt CycleInt::operator++()
{
    number++;
    if(number >= limit) number -= limit;

    return *this;
}

CycleInt CycleInt::operator--()
{
    number--;
    if(number < 0) number += limit;

    return *this;
}

CycleInt CycleInt::operator+(CycleInt o)
{
    CycleInt t;
    t.number = number + o.number;
    t.limit = limit;

    while(number >= limit) {
        number -= limit;
    }
    while(number < 0) {
        number += limit;
    }

    return t;
}

CycleInt CycleInt::operator+(int num)
{
    CycleInt t;
    t.number = number + num;
    t.limit = limit;

    while(number >= limit) {
        number -= limit;
    }
    while(number < 0) {
        number += limit;
    }

    return t;
}

CycleInt CycleInt::operator-(CycleInt o)
{
    CycleInt t;
    t.number = number - o.number;
    t.limit = limit;

    while(t.number >= limit) {
        t.number -= limit;
    }
    while(t.number < 0) {
        t.number += limit;
    }

    return t;
}

CycleInt CycleInt::operator-(int num)
{
    CycleInt t;
    t.number = number - num;
    t.limit = limit;

    while(t.number >= limit) {
        t.number -= limit;
    }
    while(t.number < 0) {
        t.number += limit;
    }

    return t;
}

CycleInt::operator int() const
{
    return number;
}