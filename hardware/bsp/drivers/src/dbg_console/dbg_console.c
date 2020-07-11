#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "dbg_console/console_api.h"
#include "dbg_console/dbg_console_api.h"

#define DBG_CONSOLE_CHECK_ERR(c, e)                                            \
	{                                                                          \
		if (!(c)) {                                                            \
			return e;                                                          \
		}                                                                      \
	}

#define DBG_CONSOLE_BUF_SIZE 0x100
#define DBG_CONSOLE_FUNC_SIZE 0x40
#define DBG_CONSOLE_PARAMS_SIZE 0x80

static uint8_t s_dbg_console_buff[DBG_CONSOLE_BUF_SIZE] = {0};

static bool s_is_it_sys_key(uint8_t ch) {
	if (ch >= SYS_KEY_INSERT && ch <= SYS_KEY_UP_ARROW) {
		return true;
	}
	return false;
}

const char *log_level_to_string(dbg_console_log_t log_lvl) {
	switch (log_lvl) {
	case LOG_INFO:
		return "INFO";
	case LOG_DEBUG:
		return "DEBUG";
	case LOG_TRACE:
		return "TRACE";
	case LOG_ERROR:
		return "ERROR";
	case LOG_WARNING:
		return "WARNING";
	default:
		return NULL;
	}
}

static cmn_status_t s_show_func_list() {
	for (uint16_t i = 0; i < g_dbg_console.list_len; i++) {
		console_println((uint8_t *)"\t[%s] %s",
						g_dbg_console.funcs_list[i].function_name,
						g_dbg_console.funcs_list[i].description);
	}
	return STATUS_OK;
}

static bool s_invoke_is_func(const uint8_t *str) {
	assert(str);
	for (uint8_t i = 0; i < g_dbg_console.list_len; ++i) {
		if (!strncmp((const char *)g_dbg_console.funcs_list[i].function_name, 
			(const char *)str, 
			strlen((const char *)g_dbg_console.funcs_list[i].function_name))) {
			g_dbg_console.funcs_list[i].func_call_back((const char *)(str + strlen((const char *)g_dbg_console.funcs_list[i].function_name) + 1));
			return true;
		}
	}
	return false;
}

cmn_status_t dbg_console_initialize() { return STATUS_OK; }

cmn_status_t dbg_console_update_func_list(dbg_console_list_t *funcs_list,
										  uint16_t list_len) {
	assert(funcs_list);
	DBG_CONSOLE_CHECK_ERR(list_len > 0, LOG_ERROR);
	g_dbg_console.list_len = list_len;
	g_dbg_console.funcs_list = funcs_list;
	return STATUS_OK;
}

cmn_status_t dbg_console_set_show_list_key(uint8_t show_list_key) {
	g_dbg_console.show_list_key = show_list_key;
	return STATUS_OK;
}

cmn_status_t dbg_console_set_log_level(dbg_console_log_t log_level) {
	g_dbg_console.log_level = log_level;
	return STATUS_OK;
}

cmn_status_t dbg_console_println(dbg_console_log_t log_level, uint8_t *fmt, ...) {
	if (g_dbg_console.log_level & log_level) {
		DBG_CONSOLE_CHECK_ERR(log_level_to_string(log_level), STATUS_INV_ARG);
	} else {
		return STATUS_OK;
	}
	va_list args;
	console_print("\n\r");
	console_print("[%s]\t", log_level_to_string(log_level));
	va_start(args, fmt);
	console_print_vargs(fmt, args);
	va_end(args);
	return STATUS_OK;
}

cmn_status_t dbg_console_input_handling() {
	DBG_CONSOLE_CHECK_ERR(g_dbg_console.show_list_key, LOG_ERROR);
	console_println("%s", __FUNCTION__);
	bool is_init_type = true;
	uint8_t recived;
	while (1) {
		if (console_read_msg(&recived, 1) == STATUS_OK) {
			if (is_init_type) {
				is_init_type = false;
				console_print_symbol((uint8_t)'>');
				recived = 0;
				continue;
			}
			if (s_is_it_sys_key(recived)) {
				switch (recived) {
				case SYS_KEY_ENTER_2:
					if (s_dbg_console_buff[0] == g_dbg_console.show_list_key &&
						strlen((const char *)s_dbg_console_buff) == 1) {
						s_show_func_list();
					} else if (!s_invoke_is_func(s_dbg_console_buff) &&
							   strlen((const char *)s_dbg_console_buff) > 1) {
						console_println((uint8_t *)"Wrong command!");
					}
					memset(s_dbg_console_buff, 0, DBG_CONSOLE_BUF_SIZE);
					recived = 0;
					console_print_symbol('\n');
					console_print_symbol('\r');
					console_print_symbol((uint8_t)'>');
					break;
				default:
					break;
				}
			} else {
				console_print_symbol(recived);
				sprintf((char *)s_dbg_console_buff + strlen((const char *)s_dbg_console_buff), "%c", (const char)recived);
			}
		}
	}
}
