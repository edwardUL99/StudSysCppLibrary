#ifndef STUDENT_SYSTEM_H
#define STUDENT_SYSTEM_H

#include "Lecturer.h"
#include "Student.h"
#include "Course.h"
#include "Module.h"
#include "ModuleGrade.h"
#include "Exam.h"
#include "ExamGrade.h"
#include "Account.h"
#include "LecturerAccount.h"
#include "StudentAccount.h"
#include "DatabaseManager.h"
#include "Announcement.h"

#include <vector>
#include <string>

class StudentSystem {
    private:
        DatabaseManager database;
        void logInfo(std::string message);
        void logError(std::string message);
        void logWarning(std::string warning);
        std::string markToGrade(float moduleGrade);
        float gradeToQPV(std::string grade);
        float calculateQCS(const Student &student, const Module &module);
        static const int FACTOR; //the only factor allowed for now is 1
        

    public:
        /**
         * Constructs a StudentSystem object
         * However, the system isn't available until startSystem is called
         */
        StudentSystem();
        StudentSystem(const StudentSystem &system);
        /**
         * Starts the system by making any necessary connections to the database
         * This NEEDS to be called before the system can be used or else the program will crash
         * @param dbname the name of the database. This database must contain all the tables in schema.sql and all procedures and triggers from procedures/triggers.sql included in this library release
         * @param username the name of the user to log into the database as. This database user needs all permissions to the tables in database
         * @param password the password of the user identified by @param username
         * @param host the hostname of the server running the database
         */
        void startSystem(std::string dbname, std::string username, std::string password, std::string host);
        /**
         * Adds the specified lecturer to the database
         * @param lecturer The lecturer to add to the system
         * @returns true if this was a success, false if not
         * @throw DuplicateException if this lecturer already exists in the database
         */
        bool addLecturer(const Lecturer &lecturer);
        /**
         * Attempts to retrieve the lecturer specified by the email
         * @param email The e-mail address of the lecturer
         * @return the lecturer if successfully found
         * @throw NotFoundException if there is no lecturer with the specified e-mail address
         */
        Lecturer getLecturer(std::string email);
        /**
         * Attempts to remove the specified lecturer from the system
         * @param lecturer the lecturer to remove
         * @return true if the operation was a success, false if not
         */
        bool removeLecturer(const Lecturer &lecturer);
        /**
         * Attempts to update the lecturer identified by email and replace it with the updated lecturer
         * @param email the email of the lecturer to update
         * @param updatedLecturer the lecturer object represented the updated lecturer
         * @return true if successful, false if not. May return false because of an error or no changes to be made
         * @throw KeyMismatch if email does not equal the email of the updated lecturer
         */
        bool updateLecturer(std::string email, const Lecturer &updatedLecturer);
        /**
         * Attempts to add the specified student to the system
         * @param student the student to add to the system
         * @return bool if the operation was successful, false if not
         * @throw DuplicateException if this student already exists in the system
         */
        bool addStudent(const Student &student);
        /**
         * Attempts to retrieve the student from the system specified by the provided id
         * @param id the id of the student 
         * @return the retrieved student if found
         * @throw NotFoundException if there is no student specified by the provided id
         */
        Student getStudent(int id);
        /**
         * Attempts to remove the student from the system
         * @param student the student to remove
         * @return true if the operation was a success, false if not
         */
        bool removeStudent(const Student &student);
        /**
         * Attempts to update the student with the specified id by replacing it with the updated student
         * @param id the id of the student to update
         * @param updatedStudent the student object representing the updated student
         * @return true if the operation was a success, false if not. This may be because there are no changes to make or an error
         * @throw KeyMismatch if the id does not match the id of the updated student object
         */
        bool updateStudent(int id, const Student &updatedStudent);
        /**
         * Retrieves the id of the student with the specified email
         * @param email the e-mail address of the student
         * @return the id of the student if found, -1 if not
         * 
         * THIS IS DEPRECATED. A STUDENT EMAIL ADDRESS is id@student.mail.ie
         * To obtain the id, use id = std::atoi(email.substr(0, email.find('@')).c_str())
         */
        [[deprecated("A student email has the format id@student.mail.ie, use substr and string.find to pull out id section")]]
        int getStudentID(std::string email);
        /**
         * Attempts to add the specified course to the system
         * @param course the course to add
         * @return true if the operation was successful, false if not
         * @throw DuplicateException if the course already exists in the system
         */
        bool addCourse(const Course &course);
        /**
         * Attempts to retreve the course with the specified code from the system
         * @param code the code of the course
         * @return the retrieved course if found
         * @throw NotFoundException if there is no course with the specified code in the system
         */
        Course getCourse(std::string code);
        /**
         * Attempts to remove the specified course from the system
         * @param course the course to remove
         * @return true if the operation was a success, false if not
         */
        bool removeCourse(const Course &course);
        /**
         * Attempts to update the course specified by the code and replace it with the updated one
         * @param code the code of the course to update
         * @param updatedCourse the COurse object representing the updated course
         * @return true if the operation was a success, false if not. This may be because there are no changes to be made or an error occurred
         * @throw KeyMismatch if code does not match the code of the updated course
         */
        bool updateCourse(std::string code, const Course &updatedCourse);
        /**
         * Attempts to add the specified module to the system
         * @param module the module to add to the system
         * @return true if this operation was a success, false if not
         * @throw DuplicateException if the module already exists
         */
        bool addModule(const Module &module);
        /**
         * Attempts to retrieve the module specified by the code from the system
         * @param code the code of the module
         * @return the module if found
         * @throw NotFoundException if there is no module with the code in the system
         */
        Module getModule(std::string code);
        /**
         * Attempts to remove the specified module from the system
         * @param module the module to remove
         * @return true if this operation was a success, false if not
         */
        bool removeModule(const Module &module);
        /**
         * Attempts to update the module specified by the code and replaces it with the updated module
         * @param code the code of the module to update
         * @param updatedModule the module object representing the updated module
         * @return true if this operation was a success. False if not. This may be because there are no changes to be made or an error occurs
         * @throw KeyMismatch if the code does not match updatedModule's code
         */
        bool updateModule(std::string code, const Module &updatedModule);
        /**
         * Retrieves all the modules stored in the system
         * @return the vector of all the modules in the system
         */
        std::vector<Module> getModules();
        /**
         * Attempts to add the specified announcement to the system
         * @param announcement the announcement to add
         * @return true if the operation was a success, false if not
         * @throw DuplicateException if the system already contains this announcement
         */
        bool addAnnouncement(const Announcement &announcement);
        /**
         * Attempts to remove the specified announcement from the system
         * @param announcement the announcement to remove
         * @return true if this operation was a success, false if not
         */
        bool removeAnnouncement(const Announcement &announcement);
        /**
         * Attempts to update the announcement specified by the announcement number and module code by replacing it with the updated announcement
         * @param announcementNumber the number of the announcement 
         * @param moduleCode the code of the module this announcement belongs to
         * @param updatedAnnouncement the updated announcement object
         * @return true if this was a success, false if not. This may be because there are no changes to be made or an error occurred
         * @throw KeyMismatch if announcement number and module code do not match those of the updated announcement
         */
        bool updateAnnouncement(int announcementNumber, std::string moduleCode, const Announcement &updatedAnnouncement);
        /**
         * Gets all the announcements from the system for the specified module
         * @param module the module to retrieve announcements for
         * @return a vector of all announcements for the module
         */
        std::vector<Announcement> getModuleAnnouncements(const Module &module);
        /**
         * Gets the last updated time for the specified announcement
         * @param announcement the announcement to query the updated time for
         * @return the last updated time for this announcement
         */
        std::string getAnnouncementTime(const Announcement &announcement);
        /**
         * Method to register a student to the specified module in the System 
         * Note that you can't update a student registration directly, the best route
         * is to call unregisterStudentModule and then register with the new one
         * @param student the student to register
         * @param module the module to register the student on
         * @return true if this operation was a success, false if not
         * @throw DuplicateException if this student is already registered on the module
         */
        bool registerStudentModule(const Student &student, const Module &module);
        /**
         * Method to remove a student's registration for a certain module
         * This can also be used to update an existing registration
         * i.e unregister and then register with the new module
         * @param student the student to unregister
         * @param module the module to unregister the student from
         * @return true if this operation was a success, false if not
         */
        bool unregisterStudentModule(const Student &student, const Module &module);
        /**
         * Retrieves all the modules the student is registered on
         * @param student the student to find modules for
         * @return vector of all modules the student is registered on
         */
        std::vector<Module> getStudentRegisteredModules(const Student &student);
        /**
         * Retrieves all the students registered on the specified module
         * @param module the module to find students for
         * @return vector of all students registered on the module
         */
        std::vector<Student> getStudentsRegisteredOnModule(const Module &module);
        /**
         * Calculates the QCA for the specified student
         * @param student the student to calculate QCA for
         * @return true if QCA changed, false if not
         */
        bool calculateStudentQCA(Student &student);
        /**
         * Attempts to add the specified exam to the system
         * @param exam the exam to add
         * @return true if the operation was a success, false otherwise
         * @throw DuplicateException if the exam already exists
         */
        bool addExam(const Exam &exam); //exam may need to change
        /**
         * Attempts to retrieve the exam specified by the id
         * @param id the id of the exam
         * @return the exam if found
         * @throw NotFoundException if there is no exam with the given id
         */
        Exam getExam(int id);
        /**
         * Attempts to remove the exam from the system
         * @param exam the exam to remove
         * @return true if the operation was a success
         */
        bool removeExam(const Exam &exam);
        /**
         * Attempts to update the exam by replacing it with the updatedExam
         * @param oldExam the exam to update
         * @param updatedExam the updated replacement for oldExam
         * @return true if this was successful, false if not. This may be because there are no changes to make or an error occurred 
         */
        bool updateExam(const Exam &oldExam, const Exam &updatedExam);
        /**
         * Retrieves all the exams for the specified module
         * @param module the module to retrieve the exams for
         * @return a vector with all the exams for the specified module
         */
        std::vector<Exam> retrieveExamsByModule(const Module &module);
        /**
         * Attempts to add the specified exam grade to the system
         * @param examGrade the exam grade to add
         * @return true if the operation was a success, false if not
         * @throw DuplicateException if the system already contains this exam grade
         */
        bool addExamGrade(const ExamGrade &examGrade);
        /**
         * Retrieves the specified exam grade for the specified student for the exam
         * @param student the student to get the grade for
         * @param exam the exam to get the grade for
         * @return ExamGrade if found
         * @throw NotFoundEXception if there is no ExamGrade for that student on the specified exam
         */
        ExamGrade getExamGrade(const Student &student, const Exam &exam);
        /**
         * Queries the system to see if the specified exam has been taken by any students
         * @param exam the exam to see if it's been taken
         * @return true if the exam was taken, false if not
         */
        bool examTaken(const Exam &exam);
        /**
         * Removes the specified exam grade from the system
         * @param examGrade the exam grade to remove
         * @return true if this operation was successful
         */
        bool removeExamGrade(const ExamGrade &examGrade);
        /**
         * Attempts to update the exam grade specified by the student and exam by replacing it with the updated one
         * @param student the student of the exam grade
         * @param exam the exam of the exam grade
         * @param updatedExamGrade the exam grade object to replace the current one
         * @return true if this operation was a success, false if not. This could be because there are no changes to make or an error occurred
         */
        bool updateExamGrade(const Student &student, const Exam &exam, const ExamGrade &updatedExamGrade);
        /**
         * Retrieves the calculated module grade on the specified module for the student if exists
         * This does NOT calculate the module grade
         * @param module the module to retrieve the grade on
         * @param student the student to calculate the grade for
         * @return the calculated module grade if exists
         * @throw NotFoundException if the module grade is not found in the system
         */
        ModuleGrade getModuleGrade(const Module &module, const Student &student);
        /**
         * Calculates the individual module grade for the specified student on the specified module
         * @param module the code of the module the grade is for
         * @param student the student to calculate the grade for
         */
        void calculateModuleGrade(std::string module, const Student &student);
        /**
         * Calculates all Module grades for every student in the specified module
         * @param module the code of the module to calculate grades for
         */
        void calculateAllModuleGradesForModule(std::string module);
        /**
         * Attemots to add the specified lecturer account to the system
         * @param lecturerAccount the account to add to the system
         * @return true if this operation was a success
         * @throw DuplicateException if the account already exists in the system
         */
        bool addAccount(LecturerAccount lecturerAccount);
        /**
         * Attempts to retrieve the lecturer account for the lecturer with the specified email
         * @param email the email of the lecturer to find the account for
         * @return LecturerAccount if it exists
         * @throw NotFoundException if there is no account for the lecturer with the specified email
         */
        LecturerAccount getLecturerAccount(std::string email);
        /**
         * Attempts to remove the specified lecturer account from the system
         * @param lecturerAccount the account to remove
         * @return true if this operation was successful, false if not
         */
        bool removeAccount(const LecturerAccount &lecturerAccount);
        /**
         * Attempts to update the lecturer account for the specified lecturer by replacing it with the updated one
         * @param lecturer the lecturer to update
         * @param updatedLecturerAccount the updated lecturer account
         * @return true if this operation was a success, false if not. This may be because there are no changes to make or an error occurred
         * @throw KeyMismatch if the lecturer does not match that of the updatedLecturerAccount
         */
        bool updateAccount(const Lecturer &lecturer, const LecturerAccount &updatedLecturerAccount);
        /**
         * Attempts to add the specified student account to the system
         * @param studentAccount the account to add to the system
         * @return true if this operation was a success
         * @throw DuplicateException if the account already exists in the system
         */
        bool addAccount(const StudentAccount &studentAccount);
        /**
         * Attempts to retrieve the student account for the student with the specified id
         * @param id the id of the student
         * @return Student ccount if it exists
         * @throw NotFoundException if there is no account for the student with the specified id
         */
        StudentAccount getStudentAccount(int id);
        /**
         * Removes the specified student account
         * @param studentAccount the student account to remove
         * @return true if it was successful
         */
        bool removeAccount(const StudentAccount &studentAccount);
        /**
         * Attempts to update student account for the specified student by replacing it with the updated one
         * @param studentAccount the student account to update
         * @param updatedStudentAccount the updated student account
         * @return true if this operation was a success, false if not. This may be because there are no changes to make or an error occurred
         * @throw KeyMismatch if the student does not match that of the updatedStudentAccount
         */
        bool updateAccount(const Student &student, const StudentAccount &studentAccount);
        /**
         * Records login of a user and returns the most recent login
         * If it is the user's first login, the method returns "FIRST_LOGIN"
         * @param account the account to record login for
         * @return the most recent login for the account, if first login, it returns FIRST_LOGIN
         */
        std::string recordLogin(const Account &account);
        StudentSystem &operator=(const StudentSystem &system);
};

#endif