#include "mbed.h"
#include "Servo.h"
#include "TFmini.h"
#include <cmath>
#include <stdio.h>

Serial blue(p28,p27); //bluetooth UART
BusOut myled(LED1,LED2,LED3,LED4); // to see which number was pressed
Servo myservo(p23); // Servo constructor

//LIDAR Objects
TFmini tfmini(p9, p10);
Serial pc(USBTX, USBRX, 9600);


// Motor Control Objects
PwmOut SpeedL(p21); //Speed of Left motor
PwmOut SpeedR(p22); //Speed of Right motor
DigitalOut FwdL(p15); //Forward Left motor
DigitalOut FwdR(p16); //Forward Right motor
DigitalOut BckL(p19); //Backward Left motor
DigitalOut BckR(p18); //Backward Right motor

// Constant vars and values
#define PI 3.1415926535
#define MAX_RANGE_CM 1200   // max range in cm
#define MAP_DIM 120         // dimensions of the mapping (each side is 120x120 for a total of 240x120)
#define CM_PER_BLOCK MAX_RANGE_CM / (MAP_DIM / 2)   // number of cm per pixel (current 10 cm per pixel)

float current_angle = 0; // current angle that the LIDAR is oriented

// LIDAR CODE ----------------------------

// Returns the smaller of the given ints
int min(int a, int b) {
    return a < b ? a : b;   
}

// Clear all values within the bitmap array with 0
void clearBitmapArray(char *bitmap)
{
    for (int i = 0; i < MAP_DIM*MAP_DIM; i++)
    {
        bitmap[i] = 0;   
    }
}

// Get a measurement in cm from the LIDAR
int getDistanceMeasurementFromLidar()
{
    int dist = min(tfmini.getDistance(), MAX_RANGE_CM - 1);
    if(tfmini.getDistance() < 700) {
        for(int i = 0; i < (int)tfmini.getDistance()/5; i++) {
            //pc.printf("#");
            wait(0.001);
        }
    }
    return dist;
}

// Given a distance and angle, plot the point on the bitmap
void plotBitmapPoint(int dist, float currentAngle, char *bitmap)
{
    // Find the actual x, y coordinate (relative to the bot's current position and orientation) in cm
    float point_x = dist * cos(currentAngle * PI / 180);
    float point_y = dist * sin(currentAngle * PI / 180);
    
    // Convert this distance to a block index
    int index_x = (int) (point_x / ((float) CM_PER_BLOCK));
    int index_y = (int) (point_y / ((float) CM_PER_BLOCK));
    
    // We have to translate it to end up in the correct location
    if (currentAngle < 90) {
        // right
        index_y = MAP_DIM - 1 - index_y;    
    } else {
        // left
        index_x += MAP_DIM - 1;
        index_y = MAP_DIM - 1 - index_y;
    }
    
    bitmap[index_x + index_y * MAP_DIM] = 1;
}

void writeBitmapToPi(char *bitmap)
{
    while (!pc.readable());
    while (pc.readable()) {
        pc.getc();   
    }
    pc.printf("START");
    for (int i = 0; i < MAP_DIM*MAP_DIM; i++) {
        pc.printf("%d", bitmap[i]);
    }
    pc.printf("STOP");
}

// END LIDAR CODE ------------------------

int main()
{
    // LIDAR config
    char *bitmap = new char[MAP_DIM*MAP_DIM];
    clearBitmapArray(bitmap);
        
    int right_done = 0;     // angles 0-90 degrees
    int left_done = 0;      // angles 90-180 degrees
    
    // servo config
    myservo.calibrate(0.0010, 45.0);
    char bnum=0;
    char bhit=0;
    while (1) {
                if(blue.readable())
                {
                    if (blue.getc()=='!') {
                        if (blue.getc()=='B') { //button data packet
                            bnum = blue.getc(); //button number
                            bhit = blue.getc(); //1=hit, 0=release
                                myled = bnum - '0'; //current button number will appear on LEDs
                                switch (bnum) {
                                    case '1': //number button 1 -- SERVO CONTROL
                                    if (bhit == '1')
                                    {
                                        myservo = 1.0; //start servo at 1.0 (servo moves from right to left)
                                        wait(1); // make sure it gets set
                                        
                                        current_angle = 0; // reset all flags
                                        right_done = 0;
                                        left_done = 0;
                                        
                                        for(float p=1.0; p>=0.0; p -= 0.01) // move 1.8 deg. at a time
                                        {
                                            myservo = p;
                                            wait(0.05);
                                            
                                            
                                            // START DISTANCE READ LOGIC
                                            
                                            int dist = getDistanceMeasurementFromLidar();
        
                                            plotBitmapPoint(dist, current_angle, bitmap);
                                            
                                            
                                            if (current_angle >= 90 && !right_done) {
                                                right_done = 1;
                                                writeBitmapToPi(bitmap);
                                                clearBitmapArray(bitmap);
                                            }
                                            
                                            if (current_angle >= 180 && !left_done) {
                                                left_done = 1;
                                                writeBitmapToPi(bitmap);
                                                clearBitmapArray(bitmap);
                                            }
                                            
                                            current_angle += 1.8;   // simulating a sweep with the servo, angle granularity is 1.8 degrees
                                            
                                            // END DISTANCE READ LOGIC                                     
                                            
                                            wait(0.05);
                                        }  
                                    }
                                    else // do nothing on servo button release
                                    {
                                        myled = bnum - '0';
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
                                            
                                            wait(0.1);
                                            
                                            SpeedL = 1.0;
                                            SpeedR = 1.0;
                                                                                
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
                                            wait(0.1);
                                            
                                            SpeedL = 1.0;
                                            SpeedR = 1.0;
                                        } 
                                        else // on release want to stop
                                        {
                                             SpeedL = 0;
                                             SpeedR = 0;
                                        }
                                        break;
                                    case '6': //button 7 left arrow
                                        if (bhit=='1') {
                                            SpeedL = 0.75;
                                            SpeedR = 0.75;
                                            FwdL = 0;
                                            BckR = 0;
                                            BckL = 1;
                                            FwdR = 1;
                                            wait(0.1);
                                            
                                            SpeedL = 1.0;
                                            SpeedR = 1.0;
                                        }
                                        else // on release want to stop
                                        {
                                             SpeedL = 0;
                                             SpeedR = 0;
                                        }
                                        break;
                                    case '5': //button 8 right arrow
                                        if (bhit=='1') {
                                            SpeedL = 0.75;
                                            SpeedR = 0.75;
                                            FwdR = 0;
                                            BckL = 0;
                                            FwdL = 1;
                                            BckR = 1;
                                            wait(0.1);
                                            
                                            SpeedL = 1.0;
                                            SpeedR = 1.0;
                                        }
                                        else // on release want to stop
                                        {
                                             SpeedL = 0;
                                             SpeedR = 0;
                                        }
                                        break;
                                    default:
                                        break;
                                }
                        }
            } // end else
            blue.getc(); //throw away checksum
        } // end readable
    } //end while
} // end main