#ifndef TFMINI_H
#define TFMINI_H

#include "mbed.h"
#include "CycleInt.h"

#define SERIAL_BUFFER_SIZE 9
#define HEADER_FIRST_BYTE 0x59
#define HEADER_SECOND_BYTE 0x59
#define RECEIVE_FREQ 0.01
#define DEFAULT_BAUD 115200

class TFmini : public RawSerial {
    public :
        TFmini(PinName serialTX, PinName serialRX);

        int getDistance();
        int getStrength();

    private :
        void readData();
        void assemble();

        CycleInt readCounter;
        CycleInt takeCounter;
        Ticker assembleTicker;
        
        int distance;
        int strength;

        uint8_t *buffer;
        uint8_t *data;

        bool headerCheck;
        int headerPoint;
};

#endif