delimiter //

/*This may not be correct, draw out example tables with data and step through each data following this procedure*/
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

        IF (SELECT COUNT(*) FROM module_grades WHERE module_grades.module = module AND module_grades.student = student) = 0 THEN
            INSERT INTO module_grades (module, student, mark) VALUES (module, student, mark);
        ELSE
            UPDATE module_grades
            SET module_grades.mark = mark
            WHERE module_grades.module = module AND module_grades.student = student;
        END IF;
    END;//

delimiter ;