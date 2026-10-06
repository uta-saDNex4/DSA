#ifndef GRADE_REPOSITORY_H
#define GRADE_REPOSITORY_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <sql.h>
#include <sqlext.h>

#include <string>
#include <vector>
struct GradeRecord {
    std::string studentID;
    std::string lastName;
    std::string firstName;
    double score;
    bool hasScore;
};

class GradeRepository {
public:
    explicit GradeRepository(SQLHDBC connection);

    std::vector<GradeRecord>
    getGradesByCreditClass(int creditClassID);

    bool updateGrade(
        int creditClassID,
        const std::string& studentID,
        double score
    );

private:
    SQLHDBC connection;
};

#endif