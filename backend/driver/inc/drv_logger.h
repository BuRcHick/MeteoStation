#ifndef _DRV_LOGGER_H_
#define _DRV_LOGGER_H_
#include "drv_common_errors.h"
#include "stdint.h"

typedef enum { LOG_INFO, LOG_ERROR, LOG_DEBUG } drv_logger_msg_type_t;

void drv_logger_print(uint8_t *buff, drv_logger_msg_type_t status);


#endif	//_LOGGER_H_