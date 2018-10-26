/* Includes */
#include "mbed.h"
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_magneto.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_accelero.h"

#include "Features.h"
#include <iostream>
using namespace std;


DigitalOut led(LED1);

/* Defines */
#define BUFFER_SIZE 10


/*Declaration */

void checkData();
void controlServo(float pGyroDataXYZ[3]);
void read_magnetometer();
void read_gyro_accelerometer();

/* Global variables declaration */
int16_t pDataXYZ[3] = {0};
float pGyroDataXYZ[3] = {0};
    
//magnetometer buffers
CircularBuffer<int16_t, BUFFER_SIZE> magnoXBuf;
CircularBuffer<int16_t, BUFFER_SIZE> magnoYBuf;
CircularBuffer<int16_t, BUFFER_SIZE> magnoZBuf;

//accelerometer buffers
CircularBuffer<int16_t, BUFFER_SIZE> acceloXBuf;
CircularBuffer<int16_t, BUFFER_SIZE> acceloYBuf;
CircularBuffer<int16_t, BUFFER_SIZE> acceloZBuf;

//gyro buffers
CircularBuffer<float, BUFFER_SIZE> gyroXBuf;
CircularBuffer<float, BUFFER_SIZE> gyroYBuf;
CircularBuffer<float, BUFFER_SIZE> gyroZBuf;

//Configure PwmOut
PwmOut _pwm(D0);

/**
 * Main function. Initialize the sensors and set up the event queue
 */
int main() {

    //Features example
    Features f;
    float data[] = {1,2,3,4,5,6,7,8,9,10};
    printf("The mean is %f, std is %f, avgDev is %f", f.mean(data, BUFFER_SIZE), f.standardDev(data, BUFFER_SIZE), f.avgDev(data, BUFFER_SIZE));

    //Initiate the sensors
    BSP_MAGNETO_Init();
    BSP_GYRO_Init();
    BSP_ACCELERO_Init();

    //Configure eventqueue 
    EventQueue queue;
    queue.call_every(20, checkData);
    queue.call_every(25, read_magnetometer);
    queue.call_every(80, read_gyro_accelerometer);
    queue.dispatch(-1);
}

/**
 * Check the buffers and decide if we have an attack
 */ 
void checkData() {
    //TODO: Completes
}

/**
 * Give the data that had been collected from the gyro
 * and use it to control the servo engine
 * @param pGyroDataXYZ array of the data from the gyro. On 3 axes
 */
void controlServo(float pGyroDataXYZ[3]){
     float xGyro = pGyroDataXYZ[0];
     _pwm.pulsewidth_us(1500+xGyro/1000.0);
}

/**
 * Read the data from the magnetometer
 */
void read_magnetometer(){
     BSP_MAGNETO_GetXYZ(pDataXYZ);
        magnoXBuf.push(pDataXYZ[0]);
        magnoYBuf.push(pDataXYZ[1]);
        magnoZBuf.push(pDataXYZ[2]);
}

/**
 * Read the data from the accelerometer and from the gyro
 */
void read_gyro_accelerometer(){
    BSP_ACCELERO_AccGetXYZ(pDataXYZ);
        acceloXBuf.push(pDataXYZ[0]);
        acceloYBuf.push(pDataXYZ[1]);
        acceloZBuf.push(pDataXYZ[2]);

        BSP_GYRO_GetXYZ(pGyroDataXYZ);
        gyroXBuf.push(pGyroDataXYZ[0]);
        gyroYBuf.push(pGyroDataXYZ[1]);
        gyroZBuf.push(pGyroDataXYZ[2]);
        
        controlServo(pGyroDataXYZ);
}


