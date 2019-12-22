#ifndef _DRV_COMMON_ERRORS_H_
#define _DRV_COMMON_ERRORS_H_
#include "stdlib.h"

typedef enum { drv_error, drv_success, drv_init_error } drv_common_errors_t;

const char* drv_error2string(drv_common_errors_t code);

#endif	//_DRV_COMMON_ERRORS_H_