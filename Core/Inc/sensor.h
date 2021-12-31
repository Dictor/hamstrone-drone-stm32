#ifndef HAMSTRONE_SENSOR_VALUE_H
#define HAMSTRONE_SENSOR_VALUE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Hamstrone header */
#include "main.h"
//#include "hamster_tongue.h"
#include "config.h"
#include "serialize.h"

int SPIReadSingle(HAMSTRONE_SPI_HANDLE_TYPE *fd, uint8_t regaddr, uint8_t *buf);
int SPIRead(HAMSTRONE_SPI_HANDLE_TYPE *fd, uint8_t regaddr, uint8_t recieveBytes, uint8_t *val);
int SPIWriteSingle(HAMSTRONE_SPI_HANDLE_TYPE *fd, uint8_t regaddr, uint8_t value);
int I2CWriteRegisterSingle(HAMSTRONE_I2C_HANDLE_TYPE *fd, uint16_t addr, uint8_t regaddr, uint8_t value);
int I2CWriteSingle(HAMSTRONE_I2C_HANDLE_TYPE *fd, uint16_t addr, uint8_t value);
int I2CReadSingle(HAMSTRONE_I2C_HANDLE_TYPE *fd, uint16_t addr, uint8_t regaddr, uint8_t *buf);
int I2CRead(HAMSTRONE_I2C_HANDLE_TYPE *fd, uint16_t addr, uint8_t length, uint8_t *buf);
int TCA9548SetChannel(HAMSTRONE_I2C_HANDLE_TYPE *fd, uint8_t chan);

#ifdef __cplusplus
}
#endif

#endif
