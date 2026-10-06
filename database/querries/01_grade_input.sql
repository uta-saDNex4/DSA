-- =============================================
-- 01_grade_input.sql
-- Function i: Input / Update Scores
-- =============================================

USE StudentGradeManagement;
GO


-- =============================================
-- 1. FIND CREDIT CLASS
-- =============================================

SELECT
    cc.CreditClassID,
    cc.AcademicYear,
    cc.Semester,
    cc.SubjectID,
    sub.SubjectName,
    cc.GroupNumber
FROM CreditClasses cc
JOIN Subjects sub
    ON cc.SubjectID = sub.SubjectID
WHERE cc.AcademicYear = '2026-2027'
  AND cc.Semester = 1
  AND cc.SubjectID = 'MH001'
  AND cc.GroupNumber = 1
  AND cc.IsCancelled = 0;
GO


-- =============================================
-- 2. DISPLAY STUDENTS AND SCORES
-- =============================================

SELECT
    ROW_NUMBER() OVER (
        ORDER BY sv.StudentID
    ) AS STT,

    sv.StudentID AS MASV,
    sv.LastName AS HO,
    sv.FirstName AS TEN,
    r.Score AS DIEM

FROM Registrations r

JOIN Students sv
    ON r.StudentID = sv.StudentID

WHERE r.CreditClassID = 1
  AND r.IsCancelled = 0

ORDER BY sv.StudentID;
GO


-- =============================================
-- 3. UPDATE SCORE
-- Example: SV003 gets 8.00
-- =============================================

UPDATE Registrations
SET Score = 8.00
WHERE CreditClassID = 1
  AND StudentID = 'SV003';
GO


-- =============================================
-- 4. CHECK UPDATED SCORE
-- =============================================

SELECT
    sv.StudentID AS MASV,
    sv.LastName AS HO,
    sv.FirstName AS TEN,
    r.Score AS DIEM
FROM Registrations r

JOIN Students sv
    ON r.StudentID = sv.StudentID

WHERE r.CreditClassID = 1
  AND r.IsCancelled = 0

ORDER BY sv.StudentID;
GO