#include "hamstrone_task.h"

void tskTransmitValue(void *args)
{
    int period  = 100; //100ms
    HAMSTERTONGUE_Message *msg = HAMSTERTONGUE_NewMessage(HAMSTERTONGUE_MESSAGE_VERB_VALUE, 0, sizeof(HAMSTRONE_CONFIG_VALUE_TYPE) * HAMSTRONE_CONFIG_VALUE_SIZE);

    while (1)
    {
        HAMSTRONE_Serialize32Array(HAMSTRONE_GetValueStorePointer(), msg->Payload, HAMSTRONE_CONFIG_VALUE_SIZE, 0);
        HAMSTERTONGUE_WriteMessage(HAMSTRONE_GLOBAL_TELEMETRY_PORT, msg);
        osDelay(period);
    }
}

#define SO6203_COUNT 0
#define TFMINI_COUNT 0
void tskUpdateValue(void *args)
{
    int period = 4; //4ms
    uint32_t startTick, currentTick, taskendTick;
    uint32_t tickFreq = osKernelGetTickFreq();
    startTick = osKernelGetTickCount();
    HAMSTERTONGUE_Debugf("start tick : %d", startTick);

    mpu9250Data mpudata;
    double angle[3], pidangle[3];
    uint16_t motor[4] = {0, 0, 0, 0};
    uint16_t bright[SO6203_COUNT], dist[TFMINI_COUNT];

    if (initTF() < 0) {
    	HAMSTERTONGUE_WriteAndFreeMessage(
    	            HAMSTRONE_GLOBAL_TELEMETRY_PORT,
    	            HAMSTERTONGUE_NewFormatStringMessage(
    	                HAMSTERTONGUE_MESSAGE_VERB_SIGNAL,
    	                HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_SENSORINITFAIL,
    	                32,
    	                "init tensorflow"));
    }

    /* initialize SO6203 */
    if (initSO6203(0, 0 + SO6203_COUNT) < 0)
    {
        HAMSTERTONGUE_WriteAndFreeMessage(
            HAMSTRONE_GLOBAL_TELEMETRY_PORT,
            HAMSTERTONGUE_NewFormatStringMessage(
                HAMSTERTONGUE_MESSAGE_VERB_SIGNAL,
                HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_SENSORINITFAIL,
                32,
                "init so6203"));
    }

    if (initMPU9250() < 0)
    {
        HAMSTERTONGUE_WriteAndFreeMessage(
            HAMSTRONE_GLOBAL_TELEMETRY_PORT,
            HAMSTERTONGUE_NewFormatStringMessage(
                HAMSTERTONGUE_MESSAGE_VERB_SIGNAL,
                HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_SENSORINITFAIL,
                16,
                "init mpu9250"));
        return;
    }

    InitMotor(HAMSTRONE_GLOBAL_MOTOR_PWM);

    while (1)
    {
        /* update runtime */
    	currentTick = osKernelGetTickCount();
        HAMSTRONE_WriteValueStore(0, (uint32_t)((currentTick - startTick) / tickFreq));

        /* update so6203 sensor value */
        /*
        if (readSO6203(0, SO6203_COUNT, bright) < 0)
        {
            HAMSTERTONGUE_WriteAndFreeMessage(
                HAMSTRONE_GLOBAL_TELEMETRY_PORT,
                HAMSTERTONGUE_NewFormatStringMessage(
                    HAMSTERTONGUE_MESSAGE_VERB_SIGNAL,
                    HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_SENSORREADFAIL,
                    16,
                    "read so6203"));
        }
        for (int i = 0; i < SO6203_COUNT; i++)
        {
            HAMSTRONE_WriteValueStore(10 + i, (uint32_t)bright[i]);
        }
*/
        /* update tfmini sensor value */
        /*
        if (readSO6203(0, TFMINI_COUNT, dist) < 0)
        {
            HAMSTERTONGUE_WriteAndFreeMessage(
                HAMSTRONE_GLOBAL_TELEMETRY_PORT,
                HAMSTERTONGUE_NewFormatStringMessage(
                    HAMSTERTONGUE_MESSAGE_VERB_SIGNAL,
                    HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_SENSORREADFAIL,
                    16,
                    "read tfmini"));
        }
        for (int i = 0; i < TFMINI_COUNT; i++)
        {
            HAMSTRONE_WriteValueStore(10 + TFMINI_COUNT + i, (uint32_t)dist[i]);
        }
*/

        /* update mpu9250 sensor value */
        if (readMPU9250(&mpudata) < 0)
        {
            HAMSTERTONGUE_WriteAndFreeMessage(
                HAMSTRONE_GLOBAL_TELEMETRY_PORT,
                HAMSTERTONGUE_NewFormatStringMessage(
                    HAMSTERTONGUE_MESSAGE_VERB_SIGNAL,
                    HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_SENSORREADFAIL,
                    16,
                    "read mpu9250"));
        }

        /* calculate posture */
        updateKalman(&mpudata, angle);
        //updateComplimentary(&mpudata, angle);
        HAMSTRONE_WriteValueStore(2, (uint32_t)(angle[0] * 100 + 18000));
        HAMSTRONE_WriteValueStore(3, (uint32_t)(angle[1] * 100 + 18000));
        HAMSTRONE_WriteValueStore(4, (uint32_t)(angle[2] * 100 + 18000));
        HAMSTRONE_WriteValueStore(5, (uint32_t)(mpudata.temp * 100 + 10000));

        /* process pid control*/
        updatePID(angle[0], angle[1], pidangle);
        motor[0] = 2 * (pidangle[0] + pidangle[1]) + 100;
        motor[1] = 2 * (pidangle[0] - pidangle[1]) + 100;
        motor[2] = 2 * (-pidangle[0] + pidangle[1]) + 100;
        motor[3] = 2 * (-pidangle[0] - pidangle[1]) + 100;

        PWMWriteAll(HAMSTRONE_GLOBAL_MOTOR_PWM, motor[0], motor[1], motor[2], motor[3]);
        HAMSTRONE_WriteValueStore(6, (uint32_t)motor[0]);
        HAMSTRONE_WriteValueStore(7, (uint32_t)motor[1]);
        HAMSTRONE_WriteValueStore(8, (uint32_t)motor[2]);
        HAMSTRONE_WriteValueStore(9, (uint32_t)motor[3]);

        osDelay(period);
        taskendTick = osKernelGetTickCount();

        // PROPERY TICK RESOULUTION IS SMALLER THAN 1000USEC
        HAMSTRONE_WriteValueStore(1, (uint32_t)(((float)(taskendTick - currentTick) / (float)tickFreq) * 1000));
    }
}

void tskInference(void *args)
{
    uint8_t qNetInput[7] = {0,};
    float qNetResult;
	int period = 3000;
	while(1) {
	    /* interence */
	    if (inferenceModel(qNetInput, 8, &qNetResult) < 0) {
	    	HAMSTERTONGUE_WriteAndFreeMessage(
	    	                HAMSTRONE_GLOBAL_TELEMETRY_PORT,
	    	                HAMSTERTONGUE_NewFormatStringMessage(
	    	                    HAMSTERTONGUE_MESSAGE_VERB_SIGNAL,
	    	                    HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_SENSORREADFAIL,
	    	                    24,
	    	                    "inference model"));
	    }
	    HAMSTRONE_WriteValueStore(15, (uint32_t)qNetResult);
	    osDelay(period);
	}
}
