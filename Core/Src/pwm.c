#include "pwm.h"

// duty is uint16_t, 0~65535
#define DUTY_TO_VALUE(d) (uint16_t)(d / 65535.0) * PWM_PERIOD;

void InitMotor(HAMSTRONE_TIM_HANDLE_TYPE *hdl)
{
	HAL_TIM_PWM_Init(hdl);
	HAL_TIM_PWM_Start(hdl, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(hdl, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(hdl, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(hdl, TIM_CHANNEL_4);

	hdl->Instance->CCR1 = DUTY_TO_VALUE(ESC_DUTY_MAX);
	hdl->Instance->CCR2 = DUTY_TO_VALUE(ESC_DUTY_MAX);
	hdl->Instance->CCR3 = DUTY_TO_VALUE(ESC_DUTY_MAX);
	hdl->Instance->CCR4 = DUTY_TO_VALUE(ESC_DUTY_MAX);

    osDelay(1000U);
    PWMWriteAll(hdl, 0, 0, 0, 0);
    osDelay(1000U);
}

uint16_t suppress(uint16_t val)
{
#define MARGIN 1500
    if (val < ESC_DUTY_MIN + MARGIN)
    {
        return ESC_DUTY_MIN + MARGIN;
    }
    else if (val > ESC_DUTY_MAX - MARGIN)
    {
        return ESC_DUTY_MAX - MARGIN;
    }
    else
    {
        return val;
    }
}

void PWMWriteAll(HAMSTRONE_TIM_HANDLE_TYPE *hdl, uint16_t perc1, uint16_t perc2, uint16_t perc3, uint16_t perc4)
{
	hdl->Instance->CCR1 = DUTY_TO_VALUE(suppress(ESC_DUTY_MIN + perc1 * ESC_DUTY_PERCENTILE_STEP));
	hdl->Instance->CCR2 = DUTY_TO_VALUE(suppress(ESC_DUTY_MIN + perc2 * ESC_DUTY_PERCENTILE_STEP));
	hdl->Instance->CCR3 = DUTY_TO_VALUE(suppress(ESC_DUTY_MIN + perc3 * ESC_DUTY_PERCENTILE_STEP));
	hdl->Instance->CCR4 = DUTY_TO_VALUE(suppress(ESC_DUTY_MIN + perc4 * ESC_DUTY_PERCENTILE_STEP));
}

/*
void PWMWriteSingle(struct pwm_info_s *info, int index, uint8_t percentile)
{
    if (percentile > 100)
        percentile = 100;
    info->channels[index].duty = ESC_DUTY_MIN + percentile * ESC_DUTY_PERCENTILE_STEP;
    ioctl(HAMSTRONE_GLOBAL_MOTOR_PWM, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
}
*/
