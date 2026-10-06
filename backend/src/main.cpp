#include <iostream>
#include <iomanip>

#include "database/Database.h"
#include "repositories/GradeRepository.h"

void printGrades(
    const std::vector<GradeRecord>& grades
)
{
    std::cout
        << "\n---------------------------------------------\n";

    std::cout
        << std::left
        << std::setw(10) << "MASV"
        << std::setw(20) << "HO"
        << std::setw(15) << "TEN"
        << "DIEM"
        << "\n";

    std::cout
        << "---------------------------------------------\n";

    for (const auto& grade : grades) {

        std::cout
            << std::left
            << std::setw(10) << grade.studentID
            << std::setw(20) << grade.lastName
            << std::setw(15) << grade.firstName;

        if (grade.hasScore) {
            std::cout << grade.score;
        }
        else {
            std::cout << "Chua co";
        }

        std::cout << "\n";
    }

    std::cout
        << "---------------------------------------------\n";
}

int main()
{
    std::cout
        << "=== Student Grade Management ===\n";

    Database database;

    if (!database.connect()) {
        return 1;
    }

    GradeRepository repository(
        database.getConnection()
    );

    // CreditClassID = 1
    const int creditClassID = 1;

    std::cout
        << "\n[DANH SACH DIEM BAN DAU]\n";

    auto grades =
        repository.getGradesByCreditClass(
            creditClassID
        );

    printGrades(grades);

    // Test update
    std::cout
        << "\n[CAP NHAT DIEM]\n";

    std::cout
        << "Cap nhat SV003 -> 8.0\n";

    bool updated =
        repository.updateGrade(
            creditClassID,
            "SV003",
            8.0
        );

    if (updated) {
        std::cout
            << "Update thanh cong!\n";
    }
    else {
        std::cout
            << "Update that bai!\n";

        return 1;
    }

    // Read again
    std::cout
        << "\n[DANH SACH DIEM SAU KHI CAP NHAT]\n";

    grades =
        repository.getGradesByCreditClass(
            creditClassID
        );

    printGrades(grades);

    return 0;
}