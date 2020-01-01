#include "drv_common_status.h"

const char* drv_error2string(drv_status_t code) {
	switch (code) {
		case drv_error:
			return "Error";
			break;
		case drv_success:
			return "Succes";
			break;
		case drv_init_error:
			return "Initial error";
			break;
		default:
			break;
	}
	return NULL;
}