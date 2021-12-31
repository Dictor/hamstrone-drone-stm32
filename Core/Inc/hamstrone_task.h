#ifndef HAMSTRONE_TASK_VALUE_H
#define HAMSTRONE_TASK_VALUE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Standard header */
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Hamstrone header */
#include "main.h"
#include "hamster_tongue.h"
#include "config.h"
#include "serialize.h"
#include "value_store.h"
#include "mpu9250.h"
#include "sensor.h"
#include "control.h"
#include "bright_dist_sensor.h"

void tskTransmitValue(void *args);
void tskUpdateValue(void *args);

#ifdef __cplusplus
}
#endif

#endif
