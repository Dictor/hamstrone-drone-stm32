#include "uart_dma.h"

UartDmaContext *newUartDmaContext(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdmarx, uint8_t buflen)
{
    UartDmaContext *ctx = malloc(sizeof(UartDmaContext));
    ctx->huart = huart;
    ctx->hdmarx = hdmarx;
    ctx->dmaRxBuf = malloc(UART_DMA_CONTEXT_DMA_BUF_SIZE);
    ctx->dmaRxBufLen = UART_DMA_CONTEXT_DMA_BUF_SIZE;
    ctx->dmaRxBufPos = 0;
    ctx->RxBuf = malloc(UART_DMA_CONTEXT_BUF_SIZE);
    ctx->RxBufLen = UART_DMA_CONTEXT_BUF_SIZE;
    ctx->RxBufPos = 0;
    ctx->RxBufSem = osSemaphoreNew(1, 1, NULL);
    return ctx;
}

void UartDmaRxInit(UartDmaContext *ctx)
{
    HAL_UARTEx_ReceiveToIdle_DMA(ctx->huart, ctx->dmaRxBuf, ctx->dmaRxBufLen);
    __HAL_DMA_DISABLE_IT(ctx->hdmarx, DMA_IT_HT);
}

void UartDmaRxRecieve(UartDmaContext *ctx, uint8_t recieveSize)
{
    osSemaphoreAcquire(ctx->RxBufSem, osWaitForever);
    if (ctx->dmaRxBufPos + recieveSize > ctx->dmaRxBufLen) // if recieve over end of buffer
    {
        uint8_t frag1size = ctx->dmaRxBufLen - ctx->dmaRxBufPos;
        uint8_t frag2size = recieveSize - frag1size;
        memcpy(ctx->dmaRxBuf + ctx->dmaRxBufPos, ctx->RxBuf + ctx->RxBufPos, frag1size);
        ctx->RxBufPos += frag1size;
        memcpy(ctx->dmaRxBuf, ctx->RxBuf + ctx->RxBufPos, frag2size);
        ctx->RxBufPos += frag2size;
        ctx->dmaRxBufPos = frag2size;
    }
    else
    {
        memcpy(ctx->dmaRxBuf + ctx->dmaRxBufPos, ctx->RxBuf + ctx->RxBufPos, recieveSize);
        ctx->dmaRxBufPos += recieveSize;
    }
    osSemaphoreRelease(ctx->RxBufSem);

    UartDmaRxInit(ctx);
}

int UartDmaRxFlush(UartDmaContext *ctx, uint8_t *target, uint8_t targetSize)
{
    if (osSemaphoreAcquire(ctx->RxBufSem, osWaitForever) != osOK)
    {
        return -1;
    }
    uint8_t ret = 0;
    if (ctx->RxBufPos > targetSize)
    {
        memcpy(ctx->RxBuf, target, targetSize);
        memmove(ctx->RxBuf + targetSize, ctx->RxBuf, ctx->RxBufPos - targetSize);
        ctx->RxBufPos = ctx->RxBufPos - targetSize;
        ret = targetSize;
    }
    else if (ctx->RxBufPos == targetSize)
    {
        memcpy(ctx->RxBuf, target, targetSize);
        ctx->RxBufPos = 0;
        ret = targetSize;
    }
    else
    {
        memcpy(ctx->RxBuf, target, ctx->dmaRxBufPos);
        ctx->RxBufPos = 0;
        ret = ctx->dmaRxBufPos;
    }
    osSemaphoreRelease(ctx->RxBufSem);
    return ret;
}
