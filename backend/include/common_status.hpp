#pragma once

typedef enum {
	cmn_success,
	error_unknown,
	error_inv_arg,
	status_timeout
}common_status_t;

const char* statusToString(common_status_t status);