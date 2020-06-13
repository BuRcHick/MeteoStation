#ifndef _DBG_CONSOLE_API_H_
#define _DBG_CONSOLE_API_H_
#include <stdint.h>
#include "common_status.h"

#define DBG_CONSOLE_START_MSG_LEN	0x100
#define DBG_CONSOLE_ITEM_BUF_LEN	0x100

typedef enum {
	SYS_KEY_INSERT = 0x01,
	SYS_KEY_HOME = 0x02,
	SYS_KEY_PG_UP = 0x03,
	SYS_KEY_DELETE = 0x04,
	SYS_KEY_END = 0x05,
	SYS_KEY_PG_DWN = 0x06,
	SYS_KEY_RIGHT_ARROW = 0x07,
	SYS_KEY_BACKSPACE = 0x08,
	SYS_KEY_TAB = 0x08,
	SYS_KEY_ENTER = 0x0A,
	SYS_KEY_LEFT_ARROW = 0x0B,
	SYS_KEY_DWN_ARROW = 0x0C,
	SYS_KEY_ENTER_2 = 0x0D,
	SYS_KEY_UP_ARROW = 0x0E
} dbg_console_sys_keys_t;

typedef enum {
	LOG_INFO = 0x01,
	LOG_DEBUG = 0x02,
	LOG_TRACE = 0x04,
	LOG_ERROR = 0x08,
	LOG_WARNING = 0x10
} dbg_console_log_t;

typedef struct {
	const char* function_name;
	const char* description;
	void (*func_call_back)(uint8_t*);
} dbg_console_list_t;

typedef struct {
	dbg_console_list_t* funcs_list;
	uint16_t list_len;
	uint8_t show_list_key;
	dbg_console_log_t log_level;
} dbg_console_t;

const char* log_level_to_string(dbg_console_log_t log_lvl);

cmn_status_t dbg_console_initialize();

cmn_status_t dbg_console_update_func_list(dbg_console_list_t* funcs_list, uint16_t list_len);

cmn_status_t dbg_console_set_show_list_key(uint8_t show_list_key);

cmn_status_t dbg_console_set_log_level(dbg_console_log_t log_level);

cmn_status_t dbg_console_println(dbg_console_log_t log_level, uint8_t* fmt, ...);

cmn_status_t dbg_console_input_handling();

extern dbg_console_t g_dbg_console;

#endif  //_DBG_CONSOLE_API_H_