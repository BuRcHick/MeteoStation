#include "drv_logger.h"
#include "stm32f4xx_hal.h"
#include "string.h"
#include "stdio.h"
#define LOGGER_TIMEOUT 		10000
#define LOGGER_BUFF_SIZE	0xFF
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

void drv_logger_print(uint8_t* buff, drv_logger_msg_type_t status) {
	sprintf((char *)logger_buff, "\n\r[%s]%s\n\r", s_status2sting(status), buff);
	HAL_UART_Transmit(&huart2, logger_buff, strlen((char *)logger_buff),
					  LOGGER_TIMEOUT);
}