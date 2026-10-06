-- =============================================
-- 02_create_tables.sql
-- Create tables for Student Grade Management
-- =============================================

USE StudentGradeManagement;
GO


-- =============================================
-- 1. STUDENT CLASSES
-- =============================================

CREATE TABLE StudentClasses (
    ClassID VARCHAR(15) PRIMARY KEY,
    ClassName NVARCHAR(51) NOT NULL
);
GO


-- =============================================
-- 2. STUDENTS
-- =============================================

CREATE TABLE Students (
    StudentID VARCHAR(16) PRIMARY KEY,
    ClassID VARCHAR(15) NOT NULL,
    LastName NVARCHAR(51) NOT NULL,
    FirstName NVARCHAR(16) NOT NULL,
    Gender NVARCHAR(10),
    Phone VARCHAR(16),

    CONSTRAINT FK_Students_StudentClasses
        FOREIGN KEY (ClassID)
        REFERENCES StudentClasses(ClassID)
);
GO


-- =============================================
-- 3. SUBJECTS
-- =============================================

CREATE TABLE Subjects (
    SubjectID VARCHAR(10) PRIMARY KEY,
    SubjectName NVARCHAR(51) NOT NULL,

    TheoryCredits INT NOT NULL DEFAULT 0,
    PracticeCredits INT NOT NULL DEFAULT 0,

    CONSTRAINT CK_Subjects_Credits
        CHECK (
            TheoryCredits >= 0
            AND PracticeCredits >= 0
        )
);
GO


-- =============================================
-- 4. CREDIT CLASSES
-- =============================================

CREATE TABLE CreditClasses (
    CreditClassID INT IDENTITY(1,1) PRIMARY KEY,

    SubjectID VARCHAR(10) NOT NULL,
    AcademicYear VARCHAR(9) NOT NULL,
    Semester INT NOT NULL,
    GroupNumber INT NOT NULL,

    MinStudents INT NOT NULL DEFAULT 0,
    MaxStudents INT NOT NULL,

    IsCancelled BIT NOT NULL DEFAULT 0,

    CONSTRAINT FK_CreditClasses_Subjects
        FOREIGN KEY (SubjectID)
        REFERENCES Subjects(SubjectID),

    CONSTRAINT CK_CreditClasses_Semester
        CHECK (Semester > 0),

    CONSTRAINT CK_CreditClasses_Students
        CHECK (
            MinStudents >= 0
            AND MaxStudents > 0
            AND MinStudents <= MaxStudents
        ),

    CONSTRAINT UQ_CreditClasses
        UNIQUE (
            SubjectID,
            AcademicYear,
            Semester,
            GroupNumber
        )
);
GO


-- =============================================
-- 5. REGISTRATIONS
-- =============================================

CREATE TABLE Registrations (
    CreditClassID INT NOT NULL,
    StudentID VARCHAR(16) NOT NULL,

    Score DECIMAL(4,2) NULL,
    IsCancelled BIT NOT NULL DEFAULT 0,

    CONSTRAINT PK_Registrations
        PRIMARY KEY (CreditClassID, StudentID),

    CONSTRAINT FK_Registrations_CreditClasses
        FOREIGN KEY (CreditClassID)
        REFERENCES CreditClasses(CreditClassID),

    CONSTRAINT FK_Registrations_Students
        FOREIGN KEY (StudentID)
        REFERENCES Students(StudentID),

    CONSTRAINT CK_Registrations_Score
        CHECK (
            Score IS NULL
            OR (Score >= 0 AND Score <= 10)
        )
);
GO