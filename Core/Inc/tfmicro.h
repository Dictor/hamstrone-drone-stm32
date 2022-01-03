/*
 * tfmicro.h
 *
 *  Created on: 2022. 1. 2.
 *      Author: kimdi
 */

#ifndef INC_TFMICRO_H_
#define INC_TFMICRO_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
	int initTF();
	int inferenceModel(uint8_t *inputValue, uint8_t inputSize, float *result);
#ifdef __cplusplus
}
#endif

#endif /* INC_TFMICRO_H_ */
