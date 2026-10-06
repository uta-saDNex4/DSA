#include "database/Database.h"

#include <iostream>

Database::Database()
    : environment(SQL_NULL_HENV),
      connection(SQL_NULL_HDBC)
{
}

Database::~Database()
{
    disconnect();
}

bool Database::connect()
{
    SQLRETURN ret;

    // 1. Tạo Environment
    ret = SQLAllocHandle(
        SQL_HANDLE_ENV,
        SQL_NULL_HANDLE,
        &environment
    );

    if (!SQL_SUCCEEDED(ret)) {
        std::cout << "Cannot allocate ODBC environment.\n";
        return false;
    }

    // 2. Sử dụng ODBC 3.x
    ret = SQLSetEnvAttr(
        environment,
        SQL_ATTR_ODBC_VERSION,
        (SQLPOINTER)SQL_OV_ODBC3,
        0
    );

    if (!SQL_SUCCEEDED(ret)) {
        std::cout << "Cannot set ODBC version.\n";
        disconnect();
        return false;
    }

    // 3. Tạo connection
    ret = SQLAllocHandle(
        SQL_HANDLE_DBC,
        environment,
        &connection
    );

    if (!SQL_SUCCEEDED(ret)) {
        std::cout << "Cannot allocate ODBC connection.\n";
        disconnect();
        return false;
    }

    // 4. Connection string
    SQLCHAR connectionString[] =
        "DRIVER={ODBC Driver 18 for SQL Server};"
        "SERVER=DESKTOP-3EFVPRQ;"
        "DATABASE=StudentGradeManagement;"
        "Trusted_Connection=yes;"
        "Encrypt=yes;"
        "TrustServerCertificate=yes;";

    SQLCHAR outputString[1024];
    SQLSMALLINT outputLength;

    // 5. Connect SQL Server
    ret = SQLDriverConnectA(
        connection,
        nullptr,
        connectionString,
        SQL_NTS,
        outputString,
        sizeof(outputString),
        &outputLength,
        SQL_DRIVER_NOPROMPT
    );

    if (!SQL_SUCCEEDED(ret)) {

        std::cout
            << "Cannot connect to SQL Server.\n";

        SQLCHAR state[6];
        SQLCHAR message[1024];
        SQLINTEGER nativeError;
        SQLSMALLINT messageLength;

        SQLGetDiagRecA(
            SQL_HANDLE_DBC,
            connection,
            1,
            state,
            &nativeError,
            message,
            sizeof(message),
            &messageLength
        );

        std::cout
            << "SQL State: "
            << state
            << "\n";

        std::cout
            << "Error: "
            << message
            << "\n";

        disconnect();

        return false;
    }

    std::cout
        << "Connected to SQL Server successfully!\n";

    return true;
}

void Database::disconnect()
{
    if (connection != SQL_NULL_HDBC) {

        SQLDisconnect(connection);

        SQLFreeHandle(
            SQL_HANDLE_DBC,
            connection
        );

        connection = SQL_NULL_HDBC;
    }

    if (environment != SQL_NULL_HENV) {

        SQLFreeHandle(
            SQL_HANDLE_ENV,
            environment
        );

        environment = SQL_NULL_HENV;
    }
}

SQLHDBC Database::getConnection()
{
    return connection;
}

bool Database::testQuery()
{
    if (connection == SQL_NULL_HDBC) {
        std::cout
            << "Database is not connected.\n";

        return false;
    }

    SQLHSTMT statement = SQL_NULL_HSTMT;

    SQLRETURN ret = SQLAllocHandle(
        SQL_HANDLE_STMT,
        connection,
        &statement
    );

    if (!SQL_SUCCEEDED(ret)) {
        std::cout
            << "Cannot create SQL statement.\n";

        return false;
    }

    SQLCHAR query[] =
        "SELECT COUNT(*) FROM Students";

    ret = SQLExecDirectA(
        statement,
        query,
        SQL_NTS
    );

    if (!SQL_SUCCEEDED(ret)) {

        std::cout
            << "Query failed.\n";

        SQLFreeHandle(
            SQL_HANDLE_STMT,
            statement
        );

        return false;
    }

    SQLINTEGER count = 0;

    ret = SQLFetch(statement);

    if (SQL_SUCCEEDED(ret)) {

        SQLGetData(
            statement,
            1,
            SQL_C_SLONG,
            &count,
            sizeof(count),
            nullptr
        );

        std::cout
            << "Number of students: "
            << count
            << "\n";
    }

    SQLFreeHandle(
        SQL_HANDLE_STMT,
        statement
    );

    return true;
}