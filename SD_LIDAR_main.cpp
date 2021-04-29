#include "mbed.h"
#include "TFmini.h"
#include <cmath>
#include <stdio.h>
#include "SDFileSystem.h" // note, SDFileSystem.h only works with the non-mbed-os version of mbed.h
 
SDFileSystem sd(p5, p6, p7, p8, "sd"); // the pinout on the mbed Cool Components workshop board
DigitalOut led1(LED1);
TFmini tfmini(p13, p14);
Serial pc(USBTX, USBRX, 9600);

// Constant vars and values
#define PI 3.1415926535
#define MAX_RANGE_CM 1200   // max range in cm
#define MAP_DIM 120         // dimensions of the mapping (each side is 120x120 for a total of 240x120)
#define CM_PER_BLOCK MAX_RANGE_CM / (MAP_DIM / 2)   // number of cm per pixel (current 10 cm per pixel)

float current_angle = 0; // current angle that the LIDAR is oriented

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
    pc.printf("distance<%5d cm> strength<%5d>\r\n", dist, tfmini.getDistance());
    if(tfmini.getDistance() < 700) {
        for(int i = 0; i < (int)tfmini.getDistance()/5; i++) {
            pc.printf("#");
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
    //pc.printf("\nAngle: %f, X: %f, Y: %f\n", currentAngle, point_x, point_y);
    
    pc.printf("\nX: %d, Y: %d\n", (int) (point_x / ((float) CM_PER_BLOCK)), (int) (point_y / ((float) CM_PER_BLOCK)));
    
    // Convert this distance to a block index
    int index_x = (int) (point_x / ((float) CM_PER_BLOCK));
    int index_y = (int) (point_y / ((float) CM_PER_BLOCK));
    
    // We have to translate it to end up in the correct location
    //index_x += MAP_DIM / 2;
    //index_y += MAP_DIM / 2;
    if (currentAngle < 90) {
        // right
        index_y = MAP_DIM - 1 - index_y;    
    } else {
        // left
        index_x += MAP_DIM - 1;
        index_y = MAP_DIM - 1 - index_y;
    }
    
    pc.printf("\nIndex X: %d, Index Y: %d\n", index_x, index_y);
    
    bitmap[index_x + index_y * MAP_DIM] = 1;
}

// Write the bitmap array data to a file (either on local filesystem or SD card)
void writeBitmapToFile(char *bitmap, const char *filepath)
{
    pc.printf("\nWriting bitmap to file.\n");
    FILE *outFile;
    outFile = fopen(filepath, "wb");
    fwrite(bitmap, sizeof(char), MAP_DIM*MAP_DIM, outFile);
    fclose(outFile);
    pc.printf("\nComplete writing to file.\n");
}

int main() {
    char *bitmap = new char[MAP_DIM*MAP_DIM];
    clearBitmapArray(bitmap);
        
    int right_done = 0;     // angles 0-90 degrees
    int left_done = 0;      // angles 90-180 degrees
    
    while (true) {
        led1 = !led1;       // status LED to show it is running
        
        int dist = getDistanceMeasurementFromLidar();
        
        plotBitmapPoint(dist, current_angle, bitmap);
        
        current_angle += 1.8;   // simulating a sweep with the servo, angle granularity is 1.8 degrees
        
        if (current_angle >= 90 && !right_done) {
            right_done = 1;
            writeBitmapToFile(bitmap, "/sd/right_map.out");
            clearBitmapArray(bitmap);
        }
        
        if (current_angle >= 180 && !left_done) {
            left_done = 1;
            writeBitmapToFile(bitmap, "/sd/left_map.out");
            while (true);   
        }
        
        pc.printf("\r");
        wait(0.005);
    }
}

