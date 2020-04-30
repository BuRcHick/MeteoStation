#ifndef _LOGGER_API_HPP_
#define _LOGGER_API_HPP_
#include <stdio.h>
#include <stdint.h>
#include "common_status.hpp"

typedef enum { 
	log_dbg = 1, 
	log_info = 2, 
	log_err = 4, 
	log_trace = 8 
} log_type_t;

class CLogger {
   private:
	FILE* logFile;
	CLogger();
	~CLogger();
	uint32_t logLevel;
	static CLogger *loggerInstance;
   public:
	CLogger(const CLogger &) = delete;
	CLogger(CLogger &&) = delete;

	static CLogger *getLogger();

	common_status_t setLogLevel(log_type_t);
	common_status_t resetLogLevel(log_type_t);

	void msgToLog(const log_type_t type, const char *fmt, ...);
};

#endif  //_LOGGER_API_HPP_