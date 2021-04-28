#include "mbed.h"
#include "Servo.h"
//test

RawSerial blue(p28,p27); //bluetooth UART
BusOut myled(LED1,LED2,LED3,LED4); // to see which number was pressed
Servo myservo(p23); // Servo constructor

// Motor Control Objects
PwmOut SpeedL(p21); //Speed of Left motor
PwmOut SpeedR(p22); //Speed of Right motor
DigitalOut FwdL(p15); //Forward Left motor
DigitalOut FwdR(p16); //Forward Right motor
DigitalOut BckL(p19); //Backward Left motor
DigitalOut BckR(p18); //Backward Right motor

int main()
{
    myservo.calibrate(0.0010, 45.0);
    char bnum=0;
    char bhit=0;
    while (1) {
                if (blue.getc()=='!') {
                    if (blue.getc()=='B') { //button data packet
                        bnum = blue.getc(); //button number
                        bhit = blue.getc(); //1=hit, 0=release
                            myled = bnum - '0'; //current button number will appear on LEDs
                            switch (bnum) {
                                case '1': //number button 1 -- SERVO CONTROL
                                if (bhit == '1')
                                {
                                    myservo = 0.0; //start servo at 0.0
                                    wait(2); // make sure it gets set
                                    for(float p=0.0; p<=1.0; p += 0.01) // move 1.8 deg. at a time
                                    {
                                        myservo = p;
                                        wait(0.05);
                                    }  
                                }                                   
                                    break;
                                case '2': //number button 2
                                    break;
                                case '3': //number button 3
                                    break;
                                case '4': //number button 4
                                    break;
                                case '8': //button 5 up arrow
                                    if (bhit=='1') {
                                        SpeedL = 0.5;
                                        SpeedR = 0.5;                                        
                                        BckL = 0;
                                        BckR = 0;
                                        FwdL = 1;
                                        FwdR = 1;
                                    }
                                    else // on release want to stop
                                    {
                                         SpeedL = 0;
                                         SpeedR = 0;
                                    }
                                    break;
                                case '7': //button 6 down arrow
                                    if (bhit=='1') {
                                        SpeedL = 0.5;
                                        SpeedR = 0.5;
                                        FwdL = 0;
                                        FwdR = 0;
                                        BckL = 1;
                                        BckR = 1;
                                    } 
                                    else // on release want to stop
                                    {
                                         SpeedL = 0;
                                         SpeedR = 0;
                                    }
                                    break;
                                case '6': //button 7 left arrow --> MAYBE BATTERY ISSUE CAUSING SERVO RESET?
                                    if (bhit=='1') {
                                        SpeedL = 0.5;
                                        SpeedR = 0.5;
                                        FwdL = 0;
                                        BckR = 0;
                                        BckL = 1;
                                        FwdR = 1;
                                    }
                                    else // on release want to stop
                                    {
                                         SpeedL = 0;
                                         SpeedR = 0;
                                    }
                                    break;
                                case '5': //button 8 right arrow
                                    if (bhit=='1') {
                                        SpeedL = 0.5;
                                        SpeedR = 0.5;
                                        FwdR = 0;
                                        BckL = 0;
                                        FwdL = 1;
                                        BckR = 1;
                                    }
                                    else // on release want to stop
                                    {
                                         SpeedL = 0;
                                         SpeedR = 0;
                                    }
                                    break;
                            }
                    }
        } // end else
    } //end while
} // end Motor
