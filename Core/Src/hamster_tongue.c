#include "hamster_tongue.h"

HAMSTERTONGUE_SEMAPHORE_TYPE WriteSemaphore = NULL;
HAMSTERTONGUE_HANDLE_TYPE *defaultFd = NULL;
uint32_t timeout = 100;

void HAMSTERTONGUE_SetWriteSemaphore(HAMSTERTONGUE_SEMAPHORE_TYPE sem)
{
	WriteSemaphore = sem;
}

void HAMSTERTONGUE_SetDefaultFile(HAMSTERTONGUE_HANDLE_TYPE *fd)
{
	defaultFd = fd;
}

HAMSTERTONGUE_Message *HAMSTERTONGUE_NewMessage(uint8_t verb, uint8_t noun, uint8_t payloadLength)
{
	HAMSTERTONGUE_Message *msg = malloc(sizeof(HAMSTERTONGUE_Message));
	msg->Verb = verb;
	msg->Noun = noun;
	msg->PayloadLength = payloadLength;
	msg->Payload = malloc(payloadLength);
	return msg;
}

HAMSTERTONGUE_Message *HAMSTERTONGUE_NewStringMessage(uint8_t verb, uint8_t noun, char *str)
{
	int len = strlen(str);
	HAMSTERTONGUE_Message *msg = HAMSTERTONGUE_NewMessage(verb, noun, sizeof(char) * len);
	strcpy((char *)msg->Payload, str);
	return msg;
}

HAMSTERTONGUE_Message *HAMSTERTONGUE_NewFormatStringMessage(uint8_t verb, uint8_t noun, int bufSize, char *format, ...)
{
	char *str = malloc(bufSize);
	va_list vl;
	va_start(vl, format);
	vsnprintf(str, bufSize, format, vl);
	va_end(vl);
	HAMSTERTONGUE_Message *msg = HAMSTERTONGUE_NewStringMessage(verb, noun, str);
	free(str);
	return msg;
}

uint16_t HAMSTERTONGUE_GetMessageLength(HAMSTERTONGUE_Message *msg)
{
	return msg->PayloadLength + 5;
}

uint8_t *HAMSTERTONGUE_SerializeMessage(HAMSTERTONGUE_Message *msg)
{
	uint8_t *res = malloc(HAMSTERTONGUE_GetMessageLength(msg));
	res[0] = HAMSTERTONGUE_MESSAGE_MARKER;
	res[1] = HAMSTERTONGUE_GetMessageLength(msg) - 2;
	res[2] = msg->Verb;
	res[3] = msg->Noun;
	memcpy(res + 4, msg->Payload, msg->PayloadLength);
	res[4 + msg->PayloadLength] = 0; //CRC
	return res;
}

ssize_t HAMSTERTONGUE_Read(HAMSTERTONGUE_HANDLE_TYPE *fd, uint8_t *buf, uint8_t bufsize)
{
	if (HAL_UART_Receive(fd, buf, bufsize, timeout) != HAL_OK) {
		return -1;
	} else {
		return bufsize;
	}
}

HAMSTERTONGUE_Message *HAMSTERTONGUE_ReadMessage(HAMSTERTONGUE_HANDLE_TYPE *fd)
{
	HAMSTERTONGUE_Message *msg;
	uint8_t buf[64], msgpayload[256];
	uint8_t buildStatus = 0, buildLen, msglen, msgverb, msgnoun, msgcrc; // 0 = not STX yet, 1 = not Len yet, 2 = payload reading, 3 = complete
	uint8_t readlen = HAMSTERTONGUE_Read(fd, buf, 64);
	if (readlen > 0)
	{
		for (int i = 0; i < readlen; i++)
		{
			if (buildStatus == 0)
			{
				if (buf[i] == HAMSTERTONGUE_MESSAGE_MARKER)
				{
					buildStatus = 1;
				}
				else
				{
					continue;
				}
			}
			else if (buildStatus == 1)
			{
				msglen = buf[i];
				buildStatus = 2;
				memset(msgpayload, 0, 256);
				buildLen = 0;
				break;
			}
			else if (buildStatus == 2)
			{

				buildLen++;
				if (buildLen == 1)
				{
					msgverb = buf[i];
				}
				else if (buildLen == 2)
				{
					msgnoun = buf[i];
				}
				else if (buildLen > 2 && buildLen <= msglen - 1)
				{
					msgpayload[buildLen - 3] = buf[i];
				}
				else if (buildLen == msglen)
				{
					msgcrc = buf[i];
					buildStatus = 3;
				}
			}
			else if (buildStatus == 3)
			{
				uint8_t payloadLen = msglen - 3;
				msg = HAMSTERTONGUE_NewMessage(msgverb, msgnoun, payloadLen);
				msg->Payload = malloc(payloadLen);
				memcpy(msg->Payload, msgpayload, payloadLen);
				return msg;
			}
		}
	}
}

ssize_t HAMSTERTONGUE_WriteMessage(HAMSTERTONGUE_HANDLE_TYPE *fd, HAMSTERTONGUE_Message *msg)
{
	uint8_t *serialMsg = HAMSTERTONGUE_SerializeMessage(msg);
	if (WriteSemaphore != NULL)
	{
		if (osSemaphoreAcquire(WriteSemaphore, timeout) != osOK) {
			free(serialMsg);
			return -1;
		}
	}
	int ret;
	if (HAL_UART_Transmit(fd, serialMsg, HAMSTERTONGUE_GetMessageLength(msg), 50) != HAL_OK) {
		ret = -1;
	} else {
		ret = 0;
	}
	if (WriteSemaphore != NULL)
		osSemaphoreRelease(WriteSemaphore);
	free(serialMsg);
	return ret;
}

ssize_t HAMSTERTONGUE_WriteAndFreeMessage(HAMSTERTONGUE_HANDLE_TYPE *fd, HAMSTERTONGUE_Message *msg)
{
	ssize_t res = HAMSTERTONGUE_WriteMessage(fd, msg);
	free(msg->Payload);
	free(msg);
	return res;
}

ssize_t HAMSTERTONGUE_Debugf(char *format, ...)
{
	int bufSize = strlen(format) * HAMSTERTONGUE_GENERAL_BUFFER_MULTIPLY;
	char *str = malloc(bufSize);
	va_list vl;
	va_start(vl, format);
	vsnprintf(str, bufSize, format, vl);
	va_end(vl);
	HAMSTERTONGUE_Message *msg = HAMSTERTONGUE_NewStringMessage(HAMSTERTONGUE_MESSAGE_VERB_SIGNAL, HAMSTERTONGUE_MESSAGE_NOUN_SIGNAL_DEBUG, str);
	free(str);
	return HAMSTERTONGUE_WriteAndFreeMessage(defaultFd, msg);
}
