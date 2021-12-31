#ifndef HAMSTRONE_PWM_H
#define HAMSTRONE_PWM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Hamstrone header */
#include "main.h"
#include "config.h"

void InitMotor(HAMSTRONE_TIM_HANDLE_TYPE *hdl);
void PWMWriteAll(HAMSTRONE_TIM_HANDLE_TYPE *hdl, uint16_t perc1, uint16_t perc2, uint16_t perc3, uint16_t perc4);
//void PWMWriteSingle(struct pwm_info_s *info, int index, uint8_t percentile);

#ifdef __cplusplus
}
#endif

#endif
