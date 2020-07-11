#ifndef _CONSOLE_API_H_
#define _CONSOLE_API_H_
#include <stdint.h>
#include <stdarg.h>
#include "common_status.h"
#include "main.h"

cmn_status_t console_init(UART_HandleTypeDef* uart);

cmn_status_t console_println(const uint8_t* fmt, ...);

cmn_status_t console_print(const uint8_t* fmt, ...);

cmn_status_t console_print_vargs(const uint8_t* fmt, va_list args);

cmn_status_t console_print_symbol(const uint8_t chr);

cmn_status_t console_read_msg(const uint8_t* data, uint16_t size);

#endif  //_CONSOLE_API_H_