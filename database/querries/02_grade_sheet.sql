-- =============================================
-- 02_grade_sheet.sql
-- Function j: Print Grade Sheet
-- =============================================

USE StudentGradeManagement;
GO


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

JOIN CreditClasses cc
    ON r.CreditClassID = cc.CreditClassID

JOIN Subjects sub
    ON cc.SubjectID = sub.SubjectID

WHERE cc.AcademicYear = '2026-2027'
  AND cc.Semester = 1
  AND cc.SubjectID = 'MH001'
  AND cc.GroupNumber = 1

  AND r.IsCancelled = 0
  AND cc.IsCancelled = 0

ORDER BY sv.StudentID;
GO