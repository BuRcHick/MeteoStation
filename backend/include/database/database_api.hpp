#pragma once
#include <string>
#include <sqlite3.h>
#include "common_status.hpp"
class CDatabase
{
private:
    sqlite3 *db;
    static CDatabase* dbInst;
    CDatabase(/* args */);
public:

    CDatabase(const CDatabase&) = delete;
    CDatabase(CDatabase &&) = delete;

    static CDatabase* getDB();

    CDatabase& operator =(const CDatabase&) = delete;
    CDatabase operator =(CDatabase &&) = delete;

    common_status_t openDB(const std::string&);
    common_status_t writeCommand(const std::string&);
    ~CDatabase();
};
