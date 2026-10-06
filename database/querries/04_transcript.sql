-- =============================================
-- 04_transcript.sql
-- Function l: Student Transcript
-- =============================================

USE StudentGradeManagement;
GO


SELECT
    sv.StudentID AS MASV,
    sv.LastName AS HO,
    sv.FirstName AS TEN,

    MAX(
        CASE
            WHEN sub.SubjectID = 'MH001'
            THEN r.Score
        END
    ) AS [Cấu trúc dữ liệu],

    MAX(
        CASE
            WHEN sub.SubjectID = 'MH002'
            THEN r.Score
        END
    ) AS [Cơ sở dữ liệu]

FROM Students sv

JOIN Registrations r
    ON sv.StudentID = r.StudentID

JOIN CreditClasses cc
    ON r.CreditClassID = cc.CreditClassID

JOIN Subjects sub
    ON cc.SubjectID = sub.SubjectID

WHERE sv.ClassID = 'CNTT-K48'

  AND r.IsCancelled = 0
  AND cc.IsCancelled = 0

GROUP BY
    sv.StudentID,
    sv.LastName,
    sv.FirstName

ORDER BY sv.StudentID;
GO