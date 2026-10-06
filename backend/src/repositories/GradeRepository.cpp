#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include "repositories/GradeRepository.h"

#include <iostream>

GradeRepository::GradeRepository(SQLHDBC connection)
    : connection(connection)
{
}

std::vector<GradeRecord>
GradeRepository::getGradesByCreditClass(int creditClassID)
{
    std::vector<GradeRecord> result;

    SQLHSTMT statement = SQL_NULL_HSTMT;

    SQLRETURN ret = SQLAllocHandle(
        SQL_HANDLE_STMT,
        connection,
        &statement
    );

    if (!SQL_SUCCEEDED(ret)) {
        std::cout
            << "Cannot create SQL statement.\n";

        return result;
    }

    const char* query = R"(
        SELECT
            s.StudentID,
            s.LastName,
            s.FirstName,
            r.Score
        FROM Registrations r
        INNER JOIN Students s
            ON r.StudentID = s.StudentID
        WHERE r.CreditClassID = ?
          AND r.IsCancelled = 0
        ORDER BY s.StudentID
    )";

    ret = SQLPrepareA(
        statement,
        (SQLCHAR*)query,
        SQL_NTS
    );

    if (!SQL_SUCCEEDED(ret)) {
        std::cout
            << "Cannot prepare query.\n";

        SQLFreeHandle(
            SQL_HANDLE_STMT,
            statement
        );

        return result;
    }

    SQLBindParameter(
        statement,
        1,
        SQL_PARAM_INPUT,
        SQL_C_LONG,
        SQL_INTEGER,
        0,
        0,
        &creditClassID,
        0,
        nullptr
    );

    ret = SQLExecute(statement);

    if (!SQL_SUCCEEDED(ret)) {
        std::cout
            << "Failed to execute query.\n";

        SQLFreeHandle(
            SQL_HANDLE_STMT,
            statement
        );

        return result;
    }

    char studentID[17];
    char lastName[256];
    char firstName[256];

    double score = 0;
    SQLLEN scoreIndicator;

    while (SQLFetch(statement) == SQL_SUCCESS) {

        GradeRecord record;

        SQLGetData(
            statement,
            1,
            SQL_C_CHAR,
            studentID,
            sizeof(studentID),
            nullptr
        );

        SQLGetData(
            statement,
            2,
            SQL_C_CHAR,
            lastName,
            sizeof(lastName),
            nullptr
        );

        SQLGetData(
            statement,
            3,
            SQL_C_CHAR,
            firstName,
            sizeof(firstName),
            nullptr
        );

        SQLGetData(
            statement,
            4,
            SQL_C_DOUBLE,
            &score,
            sizeof(score),
            &scoreIndicator
        );

        record.studentID = studentID;
        record.lastName = lastName;
        record.firstName = firstName;

        if (scoreIndicator == SQL_NULL_DATA) {
            record.score = 0;
            record.hasScore = false;
        }
        else {
            record.score = score;
            record.hasScore = true;
        }

        result.push_back(record);
    }

    SQLFreeHandle(
        SQL_HANDLE_STMT,
        statement
    );

    return result;
}

bool GradeRepository::updateGrade(
    int creditClassID,
    const std::string& studentID,
    double score
)
{
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

    const char* query = R"(
        UPDATE Registrations
        SET Score = ?
        WHERE CreditClassID = ?
          AND StudentID = ?
          AND IsCancelled = 0
    )";

    ret = SQLPrepareA(
        statement,
        (SQLCHAR*)query,
        SQL_NTS
    );

    if (!SQL_SUCCEEDED(ret)) {
        std::cout
            << "Cannot prepare update query.\n";

        SQLFreeHandle(
            SQL_HANDLE_STMT,
            statement
        );

        return false;
    }

    // Score
    SQLBindParameter(
        statement,
        1,
        SQL_PARAM_INPUT,
        SQL_C_DOUBLE,
        SQL_DECIMAL,
        4,
        2,
        &score,
        0,
        nullptr
    );

    // CreditClassID
    SQLBindParameter(
        statement,
        2,
        SQL_PARAM_INPUT,
        SQL_C_LONG,
        SQL_INTEGER,
        0,
        0,
        &creditClassID,
        0,
        nullptr
    );

    // StudentID
    SQLLEN studentIDLength =
        static_cast<SQLLEN>(studentID.length());

    SQLBindParameter(
        statement,
        3,
        SQL_PARAM_INPUT,
        SQL_C_CHAR,
        SQL_VARCHAR,
        16,
        0,
        (SQLPOINTER)studentID.c_str(),
        static_cast<SQLLEN>(studentID.length()),
        &studentIDLength
    );

    ret = SQLExecute(statement);

    if (!SQL_SUCCEEDED(ret)) {

        std::cout
            << "Failed to update grade.\n";

        SQLFreeHandle(
            SQL_HANDLE_STMT,
            statement
        );

        return false;
    }

    SQLLEN affectedRows = 0;

    SQLRowCount(
        statement,
        &affectedRows
    );

    SQLFreeHandle(
        SQL_HANDLE_STMT,
        statement
    );

    if (affectedRows == 0) {
        std::cout
            << "No registration was updated.\n";

        return false;
    }

    return true;
}