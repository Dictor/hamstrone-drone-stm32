#include "sensor.h"

#define TIMEOUT 100

int SPIWriteSingle(HAMSTRONE_SPI_HANDLE_TYPE *fd, uint8_t regaddr, uint8_t value)
{
    uint8_t tx[2] = {regaddr, value};
    if (HAL_SPI_Transmit(fd, tx, 2, TIMEOUT) != HAL_OK) {
    	return -1;
    } else {
    	return 0;
    }
}

int SPIReadSingle(HAMSTRONE_SPI_HANDLE_TYPE *fd, enum spi_mode_e mode, uint8_t regaddr, uint8_t *val)
{

    uint8_t tx[1] = {regaddr};
    uint8_t rx[1] = {0};

    HAL_StatusTypeDef ret = HAL_SPI_TransmitReceive(fd, tx, rx, 1, TIMEOUT);
    *val = rx[1];
    if (ret != HAL_OK) {
    	return -1;
    } else {
    	return 0;
    }
}

int SPIRead(HAMSTRONE_SPI_HANDLE_TYPE *fd, uint8_t regaddr, uint8_t recieveBytes, uint8_t *val)
{
    uint8_t tx[1] = {regaddr};
    uint8_t *rx = malloc(recieveBytes);
    memset(rx, 0, recieveBytes);
    if (HAL_SPI_Transmit(fd, tx, 1, TIMEOUT) != HAL_OK) return -1;
    if (HAL_SPI_Receive(fd, rx, recieveBytes, TIMEOUT) != HAL_OK) return -1;
    memcpy(val, rx, sizeof(uint8_t) * recieveBytes);
    free(rx);
    return 0;
}

int I2CWriteSingle(HAMSTRONE_I2C_HANDLE_TYPE *fd, uint16_t addr, uint8_t value)
{
    uint8_t tx[1] = {value};
    if (HAL_I2C_Master_Transmit(fd, addr, tx, 1, TIMEOUT) != HAL_OK) {
    	return -1;
    } else {
    	return 0;
    }
}

int I2CWriteRegisterSingle(HAMSTRONE_I2C_HANDLE_TYPE *fd, uint16_t addr, uint8_t regaddr, uint8_t value)
{
    uint8_t tx[1] = {value};
    if (HAL_I2C_Mem_Write(fd, addr, regaddr, 1, tx, 1, TIMEOUT) != HAL_OK) {
        return -1;
    } else {
       	return 0;
    }
}

int I2CReadSingle(HAMSTRONE_I2C_HANDLE_TYPE *fd, uint16_t addr, uint8_t regaddr, uint8_t *buf)
{
	if (HAL_I2C_Mem_Read(fd, addr, regaddr, 1, buf, 1, TIMEOUT) != HAL_OK) {
		return -1;
	} else {
		return 0;
	}
}

int I2CRead(HAMSTRONE_I2C_HANDLE_TYPE *fd, uint16_t addr, uint8_t length, uint8_t *buf)
{
	if (HAL_I2C_Master_Receive(fd, addr, buf, length, TIMEOUT) != HAL_OK) {
		return -1;
	} else {
		return 0;
	}
}

int TCA9548SetChannel(HAMSTRONE_I2C_HANDLE_TYPE *fd, uint8_t chan)
{
    return I2CWriteRegisterSingle(fd, HAMSTRONE_CONFIG_I2C_ADDRESS_TCA9548, HAMSTRONE_CONFIG_TCA9548_CHAN, 1 << chan);
}
