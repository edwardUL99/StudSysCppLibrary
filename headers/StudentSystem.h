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
         */
        void startSystem(std::string dbname, std::string username, std::string password, std::string host);
        bool addLecturer(const Lecturer &lecturer);
        Lecturer getLecturer(std::string email);
        bool removeLecturer(const Lecturer &lecturer);
        bool updateLecturer(std::string email, const Lecturer &updatedLecturer);
        bool addStudent(const Student &student);
        Student getStudent(int id);
        bool removeStudent(const Student &student);
        bool updateStudent(int id, const Student &updatedStudent);
        int getStudentID(std::string email);
        bool addCourse(const Course &course);
        Course getCourse(std::string id);
        bool removeCourse(const Course &course);
        bool updateCourse(std::string id, const Course &updatedCourse);
        bool addModule(const Module &module);
        Module getModule(std::string code);
        bool removeModule(const Module &module);
        bool updateModule(std::string code, const Module &updatedModule);
        std::vector<Module> getModules();

        bool addAnnouncement(const Announcement &announcement);
        bool removeAnnouncement(const Announcement &announcement);
        bool updateAnnouncement(int announcementNumber, std::string moduleCode, const Announcement &updatedAnnouncement);
        std::vector<Announcement> getModuleAnnouncements(const Module &module);
        std::string getAnnouncementTime(const Announcement &announcement);
        /**
         * Method to register a student to the specified module in the System 
         * Note that you can't update a student registration directly, the best route
         * is to call unregisterStudentModule and then register with the new one
         */
        bool registerStudentModule(const Student &student, const Module &module);
        /**
         * Method to remove a student's registration for a certain module
         * This can also be used to update an existing registration
         * i.e unregister and then register with the new module
         */
        bool unregisterStudentModule(const Student &student, const Module &module);
        std::vector<Module> getStudentRegisteredModules(const Student &student);
        std::vector<Student> getStudentsRegisteredOnModule(const Module &module);
        bool addExam(const Exam &exam); //exam may need to change
        Exam getExam(int id);
        bool removeExam(const Exam &exam);
        bool updateExam(const Exam &oldExam, const Exam &updatedExam);
        std::vector<Exam> retrieveExamsByModule(const Module &module);
        bool addExamGrade(const ExamGrade &examGrade);
        ExamGrade getExamGrade(const Student &student, const Exam &exam);
        /**
         * Queries the database to see if the specified exam has been taken by any students
         */
        bool examTaken(const Exam &exam);
        bool removeExamGrade(const ExamGrade &examGrade);
        bool updateExamGrade(const Student &student, const Exam &exam, const ExamGrade &updatedExamGrade);
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
        bool addAccount(LecturerAccount lecturerAccount);
        LecturerAccount getLecturerAccount(std::string email);
        bool removeAccount(const LecturerAccount &lecturerAccount);
        bool updateAccount(const Lecturer &lecturer, const LecturerAccount &updatedLecturerAccount);
        bool addAccount(const StudentAccount &studentAccount);
        StudentAccount getStudentAccount(int id);
        bool removeAccount(const StudentAccount &studentAccount);
        bool updateAccount(const Student &student, const StudentAccount &studentAccount);
        /**
         * Records login of a user and returns the most recent login
         * If it is the user's first login, the method returns "FIRST_LOGIN"
         */
        std::string recordLogin(const Account &account);
        StudentSystem &operator=(const StudentSystem &system);
};

#endif