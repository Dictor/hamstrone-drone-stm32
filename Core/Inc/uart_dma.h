#ifndef HAMSTRONE_UART_DMA_H
#define HAMSTRONE_UART_DMA_H

#include <stdint.h>
#include "stm32l4xx_hal.h"
#include <string.h>
#include "cmsis_os2.h"

#define UART_DMA_CONTEXT_DMA_BUF_SIZE 128
#define UART_DMA_CONTEXT_BUF_SIZE 256

typedef struct _UartDmaContext
{
    UART_HandleTypeDef *huart;
    DMA_HandleTypeDef *hdmarx;
    uint8_t *dmaRxBuf;
    uint8_t dmaRxBufLen;
    uint8_t dmaRxBufPos;
    uint8_t *RxBuf;
    uint16_t RxBufLen;
    uint16_t RxBufPos;
    osSemaphoreId_t RxBufSem;
} UartDmaContext;

int UartDmaRxFlush(UartDmaContext *ctx, uint8_t *target, uint8_t targetSize);
void UartDmaRxRecieve(UartDmaContext *ctx, uint8_t recieveSize);
void UartDmaRxInit(UartDmaContext *ctx);
UartDmaContext *newUartDmaContext(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdmarx, uint8_t buflen);

#endif