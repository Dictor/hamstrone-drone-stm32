#ifndef HAMSTRONE_SERIALIZE_H
#define HAMSTRONE_SERIALIZE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

void HAMSTRONE_Serialize32(uint32_t i, uint8_t* buf, int offset);
void HAMSTRONE_Serialize32Array(uint32_t *arr, uint8_t *buf, int arrlen, int offset);

#ifdef __cplusplus
}
#endif

#endif
