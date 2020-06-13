#ifndef _COMMON_STATUS_H_
#define _COMMON_STATUS_H_

typedef enum {
	STATUS_OK,
	STATUS_ERROR,
	STATUS_INV_ARG,
	STATUS_TIMEOUT
}cmn_status_t;

const char* cmn_status_to_string(const cmn_status_t status);

#endif //_COMMON_STATUS_H_