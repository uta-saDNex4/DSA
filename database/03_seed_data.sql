-- =============================================
-- 03_seed_data.sql
-- Insert sample data
-- =============================================

USE StudentGradeManagement;
GO


-- =============================================
-- 1. STUDENT CLASSES
-- =============================================

INSERT INTO StudentClasses
    (ClassID, ClassName)
VALUES
    ('CNTT-K48', N'Công nghệ thông tin K48');
GO


-- =============================================
-- 2. STUDENTS
-- =============================================

INSERT INTO Students
    (StudentID, ClassID, LastName, FirstName, Gender, Phone)
VALUES
    ('SV001', 'CNTT-K48', N'Nguyen Van', N'An', N'Nam', NULL),
    ('SV002', 'CNTT-K48', N'Tran Minh', N'Binh', N'Nam', NULL),
    ('SV003', 'CNTT-K48', N'Le Hoang', N'Nam', N'Nam', NULL);
GO


-- =============================================
-- 3. SUBJECTS
-- =============================================

INSERT INTO Subjects
    (SubjectID, SubjectName, TheoryCredits, PracticeCredits)
VALUES
    ('MH001', N'Cấu trúc dữ liệu', 3, 1),
    ('MH002', N'Cơ sở dữ liệu', 3, 0);
GO


-- =============================================
-- 4. CREDIT CLASSES
-- =============================================

INSERT INTO CreditClasses
    (
        SubjectID,
        AcademicYear,
        Semester,
        GroupNumber,
        MinStudents,
        MaxStudents
    )
VALUES
    ('MH001', '2026-2027', 1, 1, 1, 50),
    ('MH002', '2026-2027', 1, 1, 1, 50);
GO


-- =============================================
-- 5. REGISTRATIONS + SCORES
-- =============================================

INSERT INTO Registrations
    (
        CreditClassID,
        StudentID,
        Score,
        IsCancelled
    )
VALUES
    (1, 'SV001', 8.50, 0),
    (1, 'SV002', 7.00, 0),
    (1, 'SV003', NULL, 0),

    (2, 'SV001', 9.00, 0),
    (2, 'SV002', 8.00, 0);
GO