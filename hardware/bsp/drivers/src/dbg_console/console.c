#include <assert.h>
#include <string.h>
#include "dbg_console/console_api.h"
#include "main.h"
#define CNSL_CHECK_ERR(c, e) \
	{                        \
		if (!(c)) {          \
			return e;        \
		}                    \
	}

#define CONSOLE_MSG_SIZE 0x100
#define CONSOLE_TIMEOUT 10000

static UART_HandleTypeDef* s_console_uart = NULL;
static uint8_t s_console_msg[CONSOLE_MSG_SIZE] = {0};

cmn_status_t console_init(UART_HandleTypeDef* uart) {
	assert(uart);
	s_console_uart = uart;
	return STATUS_OK;
}

cmn_status_t console_println(const uint8_t* fmt, ...) {
	assert(fmt);
	assert(s_console_uart);
	va_list valist;
	va_start(valist, fmt);
	memset(s_console_msg, 0, CONSOLE_MSG_SIZE);
	sprintf((char*)s_console_msg, "\n\r");
	vsprintf((char*)s_console_msg + strlen((char*)s_console_msg), (char*)fmt, valist);
	va_end(valist);
	CNSL_CHECK_ERR(HAL_UART_Transmit(s_console_uart, s_console_msg, strlen((char*)s_console_msg), CONSOLE_TIMEOUT) == HAL_OK, STATUS_ERROR);
	return STATUS_OK;
}

cmn_status_t console_print(const uint8_t* fmt, ...) {
	assert(fmt);
	assert(s_console_uart);
	va_list valist;
	va_start(valist, fmt);
	memset(s_console_msg, 0, CONSOLE_MSG_SIZE);
	vsprintf((char*)s_console_msg + strlen((char*)s_console_msg), (char*)fmt, valist);
	va_end(valist);
	CNSL_CHECK_ERR(HAL_UART_Transmit(s_console_uart, s_console_msg, strlen((char*)s_console_msg), CONSOLE_TIMEOUT) == HAL_OK, STATUS_ERROR);
	return STATUS_OK;
}

cmn_status_t console_print_vargs(const uint8_t* fmt, va_list vargs) {
	assert(s_console_uart);
	assert(fmt);
	memset(s_console_msg, 0, CONSOLE_MSG_SIZE);
	vsprintf((char*)s_console_msg + strlen((char*)s_console_msg), (char*)fmt, vargs);
	CNSL_CHECK_ERR(HAL_UART_Transmit(s_console_uart, s_console_msg, strlen((char*)s_console_msg), CONSOLE_TIMEOUT) == HAL_OK, STATUS_ERROR);
	return STATUS_OK;
}

cmn_status_t console_print_symbol(const uint8_t chr) {
	assert(s_console_uart);
	memset(s_console_msg, 0, CONSOLE_MSG_SIZE);
	sprintf((char*)s_console_msg, "%c",(char)chr);
	CNSL_CHECK_ERR(HAL_UART_Transmit(s_console_uart, s_console_msg, strlen((char*)s_console_msg), CONSOLE_TIMEOUT) == HAL_OK, STATUS_ERROR);
	return STATUS_OK;
}

cmn_status_t console_read_msg(const uint8_t* data, uint16_t size) {
	assert(s_console_uart);
	assert(data);
	CNSL_CHECK_ERR(size > 0, STATUS_INV_ARG);
	CNSL_CHECK_ERR(HAL_UART_Receive(s_console_uart, data, size, CONSOLE_TIMEOUT) == HAL_OK, STATUS_ERROR);
	return STATUS_OK;
}