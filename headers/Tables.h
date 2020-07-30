#ifndef TABLES_H
#define TABLES_H

/**
 * This header file is a simple one to include the 
 *  Tables enum type
 */

//consider having enum class Tables and see if it works but you'll have to use Tables::<ENUM_VALUE>
enum Tables {
        LECTURERS,
        STUDENTS,
        COURSES,
        MODULES,
        ANNOUNCEMENTS,
        STUDENT_REGISTRATIONS,
        EXAMS,
        EXAM_QUESTIONS,
        EXAM_ANSWERS,
        EXAM_GRADES,
        MODULE_GRADES,
        LECTURER_ACCOUNTS,
        STUDENT_ACCOUNTS,
        LECTURER_LOGINS,
        STUDENT_LOGINS,
        MAX //used to "iterate" through Tables
};


#endif
