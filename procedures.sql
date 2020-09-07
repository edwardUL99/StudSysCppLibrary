delimiter //

CREATE PROCEDURE calculate_grades (
    IN module CHAR(6),
    IN student INTEGER
    )
    MODIFIES SQL DATA
    BEGIN 
        DECLARE mark FLOAT;

        SELECT SUM(grade)
        INTO mark
        FROM exam_grades
        WHERE exam_grades.student = student
              AND 
              exam IN(SELECT id FROM exams WHERE exams.module = module); 

        IF mark IS NULL THEN
            SET mark = 0;
        END IF;

        IF (SELECT COUNT(*) FROM module_grades WHERE module_grades.module = module AND module_grades.student = student) = 0 THEN
            INSERT INTO module_grades (module, student, mark) VALUES (module, student, mark);
        ELSE
            UPDATE module_grades
            SET module_grades.mark = mark, date_recorded = NOW()
            WHERE module_grades.module = module AND module_grades.student = student;
        END IF;
    END;//

delimiter ;