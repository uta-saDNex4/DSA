-- =============================================
-- 03_average_score.sql
-- Function k: Weighted Average Score
-- =============================================

USE StudentGradeManagement;
GO


SELECT
    sv.StudentID AS MASV,
    sv.LastName AS HO,
    sv.FirstName AS TEN,

    CAST(
        SUM(
            r.Score *
            (sub.TheoryCredits + sub.PracticeCredits)
        )
        /
        NULLIF(
            SUM(
                sub.TheoryCredits + sub.PracticeCredits
            ),
            0
        )
        AS DECIMAL(5,2)
    ) AS DiemTrungBinh

FROM Registrations r

JOIN Students sv
    ON r.StudentID = sv.StudentID

JOIN CreditClasses cc
    ON r.CreditClassID = cc.CreditClassID

JOIN Subjects sub
    ON cc.SubjectID = sub.SubjectID

WHERE sv.ClassID = 'CNTT-K48'

  AND r.IsCancelled = 0
  AND cc.IsCancelled = 0

  AND r.Score IS NOT NULL

GROUP BY
    sv.StudentID,
    sv.LastName,
    sv.FirstName

ORDER BY sv.StudentID;
GO