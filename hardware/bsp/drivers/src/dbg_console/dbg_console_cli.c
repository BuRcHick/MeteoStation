#include "dbg_console/dbg_console_api.h"
#include "dbg_console/console_api.h"
#define DBG_CLI_LIST_ITEM(n, d, cb) {.function_name = n, .description = d, .func_call_back = cb}

static void s_help_msg(uint8_t * arg){
	console_println("This is help MSG");
}

static void s_test_func(uint8_t * arg){
	console_println("[TEST] args: %s", (char *)arg);
}

dbg_console_list_t funcs_list [] = {
	DBG_CLI_LIST_ITEM("help", "Info", s_help_msg),
	DBG_CLI_LIST_ITEM("test", "test func", s_test_func)};

dbg_console_t g_dbg_console = {
	.funcs_list = funcs_list, 
	.list_len = sizeof(funcs_list) / sizeof(funcs_list[0]),
	.show_list_key = '?'};