#ifndef HAMSTRONE_MPU9250_H
#define HAMSTRONE_MPU9250_H

#ifdef __cplusplus
 extern "C" {
#endif

// typedef on first for include order issue
typedef struct _mpu9250Data
{
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
    float magX, magY, magZ;
    float temp;
} mpu9250Data;

#include "mpu9250_register.h"
#include "hamster_tongue.h"
#include "sensor.h"
#include "config.h"

#define MPU9250_ACCEL_COEFFICIENT 16384.0
#define MPU9250_GYRO_COEFFICIENT 131.0
#define AK8963_SENSITIVITY_FACTOR ((float)0.15f)
#define MPU9250_WHOAMI_VALUE 0x71
#define AK8963_WHOAMI_VALUE 0x48
#define ERROR_INVALID_WHOAMI -1

//#define ENABLE_MPU_DEBUG_MESSAGE
#ifdef ENABLE_MPU_DEBUG_MESSAGE
#define mpudebug(...) HAMSTERTONGUE_Debugf(__VA_ARGS__)
#else
#define mpudebug(...)
#endif

void calibrateMPU9250(float *dest1, float *dest2);
int readMPU9250(mpu9250Data *data);
int initMPU9250();
void calibrateAK8963();

#ifdef __cplusplus
}
#endif

#endif
