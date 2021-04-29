#include "TFmini.h"

TFmini::TFmini(PinName serialTX, PinName serialRX) :
    RawSerial(serialTX, serialRX, DEFAULT_BAUD),
    readCounter(SERIAL_BUFFER_SIZE, 0),
    takeCounter(SERIAL_BUFFER_SIZE, 0),
    distance(0)
{
    buffer = new uint8_t[SERIAL_BUFFER_SIZE];
    data = new uint8_t[2];
    attach(callback(this, &TFmini::readData));
    assembleTicker.attach(callback(this, &TFmini::assemble), RECEIVE_FREQ);
}

void TFmini::readData()
{
    buffer[(int)readCounter] = getc();
    ++readCounter;
}

void TFmini::assemble()
{
    //Find header
    headerCheck = false;
    headerPoint = 0xff;

    for(int i = 0; i < SERIAL_BUFFER_SIZE; i++) {
        if(buffer[i] == HEADER_FIRST_BYTE) {
            takeCounter = i;
            ++takeCounter;
            if(buffer[(int)takeCounter] == HEADER_SECOND_BYTE) {
                headerCheck = true;
                headerPoint = i;
            }
        }
    }
    if(headerPoint == 0xff) {
        return;
    }

    //assemble
    takeCounter = headerPoint;  //firstheader
    ++takeCounter;  //secondheader

    ++takeCounter;
    data[0] = buffer[(int)takeCounter];
    ++takeCounter;
    data[1] = buffer[(int)takeCounter];
    distance = ((data[1]<<8)|data[0]);
    
    ++takeCounter;
    data[0] = buffer[(int)takeCounter];
    ++takeCounter;
    data[1] = buffer[(int)takeCounter];
    strength = ((data[1]<<8)|data[0]);
}

int TFmini::getDistance()
{
    return distance;
}