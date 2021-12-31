#ifndef HAMSTRONE_BRDTSENSOR_H
#define HAMSTRONE_BRDTSENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "config.h"
#include "sensor.h"

#define ERROR_READ_FAIL -1

int readSO6203(uint8_t chanStart, uint8_t chanEnd, uint16_t *result);
int initSO6203(uint8_t chanStart, uint8_t chanEnd);
int readTFmini(uint8_t chanStart, uint8_t chanEnd, uint16_t *result);


#ifdef __cplusplus
}
#endif

#endif
