#ifndef HAMSTERTONGUE_H
#define HAMSTERTONGUE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Standard header */
#include <unistd.h>
#include <stdint.h>
//#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* Hamstrone header */
#include "config.h"

/* CMSIS& HAL header */
#include "cmsis_os2.h"
#include "stm32l4xx_hal.h"

/* MESSAGE STRUCTURE */
#define HAMSTERTONGUE_MESSAGE_MARKER 0xFF

/* MESSAGE VERB */
#define HAMSTERTONGUE_MESSAGE_VERB_HEARTBEAT 0x00
#define HAMSTERTONGUE_MESSAGE_VERB_VALUE 0x01
#define HAMSTERTONGUE_MESSAGE_VERB_SIGNAL 0x02

/* MESSAGE NOUN */
#define HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_INITOK 0x00
#define HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_INITFAIL 0x01
#define HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_DEBUG 0x03
#define HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_I2CREADFAIL 0x03
#define HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_SPIREADFAIL 0x04
#define HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_SENSORINITFAIL 0x05
#define HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_SENSORREADFAIL 0x06

#define HAMSTERTONGUE_GENERAL_BUFFER_MULTIPLY 20

#define HAMSTERTONGUE_SEMAPHORE_TYPE osSemaphoreId_t
#define HAMSTERTONGUE_HANDLE_TYPE UART_HandleTypeDef
/*
message serialize format: 
Name        Marker  Length  Verb    Noun    Payload     CRC    
Size(byte)  1       1       1       1       ~255        1
            <-              Message Length              ->
                            <-      Length              ->
*/

typedef struct _HAMSTERTONGUE_Message
{
    uint8_t Verb;
    uint8_t Noun;
    uint8_t *Payload;
    uint8_t PayloadLength;
} HAMSTERTONGUE_Message;


void HAMSTERTONGUE_SetWriteSemaphore(HAMSTERTONGUE_SEMAPHORE_TYPE sem);
void HAMSTERTONGUE_SetDefaultFile(HAMSTERTONGUE_HANDLE_TYPE *fd);
HAMSTERTONGUE_Message *HAMSTERTONGUE_NewMessage(uint8_t verb, uint8_t noun, uint8_t payloadLength);
HAMSTERTONGUE_Message *HAMSTERTONGUE_NewStringMessage(uint8_t verb, uint8_t noun, char *str);
HAMSTERTONGUE_Message *HAMSTERTONGUE_NewFormatStringMessage(uint8_t verb, uint8_t noun, int bufSize, char *format, ...);
uint16_t HAMSTERTONGUE_GetMessageLength(HAMSTERTONGUE_Message *msg);
uint8_t *HAMSTERTONGUE_SerializeMessage(HAMSTERTONGUE_Message *msg);
ssize_t HAMSTERTONGUE_WriteMessage(HAMSTERTONGUE_HANDLE_TYPE *fd, HAMSTERTONGUE_Message *msg);
ssize_t HAMSTERTONGUE_WriteAndFreeMessage(HAMSTERTONGUE_HANDLE_TYPE *fd, HAMSTERTONGUE_Message *msg);
ssize_t HAMSTERTONGUE_Debugf(char *format, ...);
ssize_t HAMSTERTONGUE_Read(HAMSTERTONGUE_HANDLE_TYPE *fd, uint8_t *buf, uint8_t bufsize);
HAMSTERTONGUE_Message *HAMSTERTONGUE_ReadMessage(HAMSTERTONGUE_HANDLE_TYPE *fd);

#ifdef __cplusplus
}
#endif

#endif

