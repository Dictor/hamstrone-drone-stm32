/*
 * tfmicro.h
 *
 *  Created on: 2022. 1. 2.
 *      Author: kimdi
 */

#ifndef INC_TFMICRO_H_
#define INC_TFMICRO_H_

#include <stdint.h>
#include "hamster_tongue.h"

#define ENABLE_TF_DEBUG_MESSAGE
#ifdef ENABLE_TF_DEBUG_MESSAGE
#define tfdebug(...) HAMSTERTONGUE_Debugf(__VA_ARGS__)
#else
#define tfdebug(...)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	int initTF();
	int inferenceModel(uint8_t *inputValue, uint8_t inputSize, float *result);
#ifdef __cplusplus
}
#endif

#endif /* INC_TFMICRO_H_ */
