#include "logger/logger_api.hpp"
#include <cstdarg>
#include <ctime>
#include <exception>
#include <sys/time.h>

static const char *s_logFileName = "dblog.log";
CLogger *CLogger::loggerInstance = nullptr;

static const char *s_logTypeToString(log_type_t type) {
	switch (type) {
	case log_err:
		return "ERROR";
	case log_info:
		return "INFO";
	case log_dbg:
		return "DEBUG";
	case log_trace:
		return "TRACE";
	default:
		return nullptr;
	}
}

const char* statusToString(common_status_t status) {
	switch (status)
	{
	case cmn_success:
		return "SUCCESS";
	case error_unknown:
		return "UNKNOWN ERROR";
	case error_inv_arg:
		return "INVALID ARGS";
	case status_timeout:
		return "TIMEOUT";
	default:
		return "UKNOWN";
	}
}

CLogger::CLogger() {
	try {
		logFile = fopen(s_logFileName, "a");
		logLevel = 0;
		fclose(logFile);
	} catch (const std::exception &e) {
		throw;
	}
}

CLogger::~CLogger() { msgToLog(log_dbg, "Logger: Delete logger"); }

CLogger *CLogger::getLogger() {
	if (!loggerInstance) {
		try {
			loggerInstance = new CLogger();
			struct loggerEraser {
				~loggerEraser() { delete loggerInstance; };
			};
			static loggerEraser logEr;

		} catch (const std::exception &e) {
			throw;
		}
	}
	return loggerInstance;
}

common_status_t CLogger::setLogLevel(log_type_t type) {
	if (log_dbg <= type && log_trace >= type) {
		logLevel |= type;
		return cmn_success;
	}
	msgToLog(log_err, "Invalid log level");
	return error_inv_arg;
}

common_status_t CLogger::resetLogLevel(log_type_t type) {
	if (log_dbg <= type && log_trace >= type) {
		logLevel ^= type;
		return cmn_success;
	}
	msgToLog(log_err, "Invalid log level");
	return error_inv_arg;
}

void CLogger::msgToLog(const log_type_t type, const char *fmt, ...) {
	if (logLevel & type) {
		logFile = fopen(s_logFileName, "a");
		timeval lcTime;
		gettimeofday(&lcTime, NULL);
		char buffer[80];
		strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&lcTime.tv_sec));
		fprintf(logFile, "%s:%03d [%s]\t", buffer, lcTime.tv_usec,
				s_logTypeToString(type));
		std::va_list varList;
		va_start(varList, fmt);
		vfprintf(logFile, fmt, varList);
		va_end(varList);
		fprintf(logFile, "\n");
		fclose(logFile);
	}
}