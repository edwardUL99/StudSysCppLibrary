#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <mysql/jdbc.h>
using namespace sql;

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/any.hpp>
#include "Tables.h"
#include "DatabaseItem.h"
#include "Warning.h"
#include "Logging.h"
#include <map>

#define DB "student_sys"
#define DB_HOST "localhost"
#define DB_USER "eddy"
#define DB_PASS ""

class Student;
class Lecturer;
class Course;
class Module;
class Announcement;
class StudentRegistration;
class Exam;
class ExamQuestion;
class ExamAnswer;
class ExamGrade;
class ModuleGrade;
class LecturerAccount;
class StudentAccount;

/**
 * The purpose of this class is to abstract out most of the database components and the database used from the rest of the system
 * i.e. abstracting out the tasks for querying or inserting data into the database
 *  Note, however methods for executing queries are still provided for any of the limited use cases
 */
class DatabaseManager
{
private:
    std::string database;
    std::string user;
    std::string pass;
    std::string host;
    bool logging;
    std::string logFileName;
    Driver *driver;
    Connection *connection;
    Statement *stmt;               //the statement to be used to execute queries
    static const std::map<Tables, std::string> tableNames;
    void clearTable(Tables table);
    void initialiseLogger();
    std::string formatQueryError(std::string query, std::string error);
    std::string getDatabaseInfoString();
    void writeToLog(logging::LogTypes type, std::string message);
    std::vector<ExamQuestion> getAllExamQuestions(int examID);
    void setLastExamID();
    void setLastAnnouncementID();
    //Goes through all Student registrations and if the lecturer of that module is null, the registration is removed
    void cleanUpNullLecturerModuleRegistrations();
    //following private methods throw their exceptions to their public callers
    //private add methods for answers and questions
    bool add(const ExamAnswer &answer);
    bool add(const ExamQuestion &question);
    //private update methods to update questions and answers (these do not throw keymismatch since keys are what will be updated)
    bool update(const ExamAnswer &oldAnswer, const ExamAnswer &newAnswer);
    bool update(const ExamQuestion &oldQuestion, const ExamQuestion &newQuestion);

public:
    /**
     * This just constructs a DatabaseManager object
     * It does NOT establish a connection to the database.
     * To do that call connectToDatabase
     */
    DatabaseManager(bool logging = true);
    DatabaseManager(const DatabaseManager &manager);
    ~DatabaseManager();
    /**
     * Connects the manager to the actual database server specified by the database parameters or left to defaults
     * It is undefined behaviour if any of this class' operations are called without this being called once before
     * In fact, it will be guaranteed to crash if it's not called
     */
    void connectToDatabase(std::string database = DB, std::string user = DB_USER, std::string pass = DB_PASS, std::string host = DB_HOST);

    //Overloaded add methods to add different entities to the database
    bool add(const Lecturer &lecturer);
    bool add(const Course &course);
    bool add(const Student &student);
    bool add(const Module &module);
    bool add(const Announcement &announcement);
    bool add(const StudentRegistration &registration);
    bool add(const Exam &exam);
    bool add(const ExamGrade &examGrade);
    bool add(const LecturerAccount &lecturerAccount);
    bool add(const StudentAccount &studentAccount);

    //Overloaded remove methods to remove different entities from the database
    bool remove(const Lecturer &lecturer);
    bool remove(const Course &course);
    bool remove(const Student &student);
    bool remove(const Module &module);
    bool remove(const Announcement &announcement);
    bool remove(const StudentRegistration &registration);
    bool remove(const Exam &exam); 
    bool remove(const ExamGrade &examGrade);
    bool remove(const LecturerAccount &lecturerAccount);
    bool remove(const StudentAccount &studentAccount);

    //Overloaded update methods to change different entities in the database, the entity must have same ID, or primary key but a different attribute, if the id (first 1 or 2 params does not match, e.g. id != updatedLecturer.getID(), KeyMismatch exception is thrown)
    bool update(std::string, const Lecturer &updatedLecturer);
    bool update(int id, const Student &updatedStudent);
    bool update(std::string id, const Course &updatedCourse);
    bool update(std::string code, const Module &updatedModule);
    bool update(int id, std::string moduleCode, const Announcement &updatedAnnouncement);
    bool update(const Exam &oldExam, const Exam &updatedExam);
    bool update(const Student &student, const Exam &exam, const ExamGrade &updatedExamGrade);
    bool update(const Lecturer &lecturer, const LecturerAccount &updatedLecturerAccount);
    bool update(const Student &student, const StudentAccount &updatedStudentAccount);

    //Various get methods to retrieve INDIVIDUAL entities from the database
    boost::optional<Lecturer> getLecturer(std::string email);
    boost::optional<Course> getCourse(std::string id);
    boost::optional<Student> getStudent(int id);
    boost::optional<Module> getModule(std::string code);
    boost::optional<StudentRegistration> getStudentRegistration(const Student &student, const Module &module);
    boost::optional<Exam> getExam(int id);
    boost::optional<ExamGrade> getExamGrade(const Student &student, const Exam &exam);
    boost::optional<ModuleGrade> getModuleGrade(const Module &module, const Student &student);
    boost::optional<LecturerAccount> getLecturerAccount(std::string email);
    boost::optional<StudentAccount> getStudentAccount(int id);

    //Various get methods to retrieve ALL entities of a type from the database
    std::vector<Lecturer> getAllLecturers();
    std::vector<Course> getAllCourses();
    std::vector<Student> getAllStudents();
    std::vector<Module> getAllModules();
    std::vector<Announcement> getAllAnnouncements();
    std::vector<StudentRegistration> getAllStudentRegistrations();
    std::vector<Exam> getAllExams();
    std::vector<ExamGrade> getAllExamGrades();
    std::vector<ModuleGrade> getAllModuleGrades();
    std::vector<LecturerAccount> getAllLecturerAccounts();
    std::vector<StudentAccount> getAllStudentAccounts();

    //Procedure to calculate module grades for the specified module and student
    void calculateModuleGrades(std::string module, const Student &student);
  
    /**
     * Checks if the database contains the specified item
     * @param item A reference to an item that's in the database
     */
    bool contains(const DatabaseItem &item);

    //Various execute methods
    ResultSet *executeQuery(std::string query);
    void execute(std::string query);
    int executeUpdate(std::string query);

    //Clears the database
    void clearDatabase();

    DatabaseManager &operator=(const DatabaseManager &manager);
    friend class StudentSystem;
};

#endif // DATABASE_MANAGER_H