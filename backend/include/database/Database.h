#ifndef DATABASE_H
#define DATABASE_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <sql.h>
#include <sqlext.h>

class Database {
public:
    Database();
    ~Database();

    bool connect();
    void disconnect();

    bool testQuery();

    // Cho Repository sử dụng connection
    SQLHDBC getConnection();

private:
    SQLHENV environment;
    SQLHDBC connection;
};

#endif