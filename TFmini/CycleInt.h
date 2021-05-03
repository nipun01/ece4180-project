#ifndef CYCLE_INT_H
#define CYCLE_INT_H

#define CYCLE_LIMIT 32

class CycleInt {
    public :
        CycleInt();
        CycleInt(unsigned int lim);
        CycleInt(unsigned int lim, int num);
        CycleInt &operator=(const CycleInt &o);
        CycleInt &operator=(const int &o);
        CycleInt operator++();
        CycleInt operator--();
        CycleInt operator+(CycleInt o);
        CycleInt operator+(int num);
        CycleInt operator-(CycleInt o);
        CycleInt operator-(int num);
        operator int() const;

    private :
        unsigned int limit;
        int number;
};

#endif