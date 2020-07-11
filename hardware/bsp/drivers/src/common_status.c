#include "common_status.h"
#include <stdlib.h>
const char* cmn_status_to_string(const cmn_status_t status) {
	switch (status) {
		case STATUS_OK:
			return "OK";
		case STATUS_ERROR:
			return "ERR";
		case STATUS_INV_ARG:
			return "INV_ARG";
		case STATUS_TIMEOUT:
			return "TIMEOUT";
		default:
			return NULL;
	}
}