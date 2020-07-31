/*any triggers go here*/

delimiter //

CREATE TRIGGER limit_duration
BEFORE INSERT ON courses
FOR EACH ROW
BEGIN
    IF NEW.duration > 5 THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Duration cannot be greater than 5';
    ELSEIF NEW.duration <= 0 THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Duration must be greater than 0';
    END IF;
END;//

delimiter //