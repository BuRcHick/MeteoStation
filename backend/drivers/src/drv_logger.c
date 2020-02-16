#include "drv_logger.h"
#include <stdarg.h>
#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "string.h"

#define LOGGER_TIMEOUT 10000
#define LOGGER_BUFF_SIZE 0x100

extern UART_HandleTypeDef huart2;
static uint8_t logger_buff[LOGGER_BUFF_SIZE];

static const char* s_status2sting(drv_logger_msg_type_t code) {
	switch (code) {
		case LOG_INFO:
			return "INFO";
			break;
		case LOG_ERROR:
			return "ERROR";
			break;
		case LOG_DEBUG:
			return "DEBUG";
			break;
		default:
			break;
	}
	return NULL;
}

void drv_logger_print(drv_logger_msg_type_t status, const uint8_t* fmt, ...) {
	va_list valist;
	va_start(valist, fmt);
	sprintf((char*)logger_buff, "\n\r[%s] ", s_status2sting(status));
	vsprintf((char*)logger_buff + strlen((char*)logger_buff), (char *)fmt, valist);
	sprintf((char*)logger_buff + strlen((char*)logger_buff), "\n\r");
	va_end(valist);
	HAL_UART_Transmit(&huart2, logger_buff, strlen((char*)logger_buff),
					  LOGGER_TIMEOUT);
}