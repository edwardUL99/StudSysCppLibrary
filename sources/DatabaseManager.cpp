#include "headers/DatabaseManager.h"
#include "headers/Warning.h"
#include "headers/Student.h"
#include "headers/Lecturer.h"
#include "headers/Course.h"
#include "headers/Module.h"
#include "headers/Announcement.h"
#include "headers/StudentRegistration.h"
#include "headers/Exam.h"
#include "headers/ExamGrade.h"
#include "headers/ModuleGrade.h"
#include "headers/LecturerAccount.h"
#include "headers/StudentAccount.h"
#include "headers/KeyMismatch.h"
#include "headers/ExamAnswer.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using std::cerr;
using std::cout;
using std::endl;
using std::map;
using std::ofstream;
using std::string;
using std::stringstream;
using std::vector;

const map<Tables, string> DatabaseManager::tableNames = {
    {LECTURERS, "lecturers"},
    {STUDENTS, "students"},
    {COURSES, "courses"},
    {MODULES, "modules"},
    {ANNOUNCEMENTS, "announcements"},
    {STUDENT_REGISTRATIONS, "student_registrations"},
    {EXAMS, "exams"},
    {EXAM_QUESTIONS, "exam_questions"},
    {EXAM_ANSWERS, "exam_answers"},
    {EXAM_GRADES, "exam_grades"},
    {MODULE_GRADES, "module_grades"},
    {LECTURER_ACCOUNTS, "lecturer_accounts"},
    {STUDENT_ACCOUNTS, "student_accounts"}};

DatabaseManager::DatabaseManager()
{
    this->connection = NULL;
    this->driver = NULL;
    this->stmt = NULL;
}

DatabaseManager::DatabaseManager(const DatabaseManager &databaseManager) {
    this->driver = NULL;
    this->connection = NULL;
    this->stmt = NULL;
    this->user = databaseManager.user;
    this->host = databaseManager.host;
    this->database = databaseManager.database;
    this->pass = databaseManager.pass;

    this->connectToDatabase(this->user, this->host, this->database, this->pass);
}

DatabaseManager::~DatabaseManager()
{
    this->writeWarningsToLog();
    delete this->stmt;
    delete this->connection;
}

void DatabaseManager::setLastExamID()
{
    ResultSet *res = executeQuery("SELECT id FROM exams WHERE id >= ALL(SELECT id FROM exams);");

    int id = 0;
    if (res->next())
    {
        id = res->getInt("id") + 1;
        if (id == 0)
            id = 1;
    }
    else
    {
        id = 1;
    }

    Exam::setLastID(id); //set the last id to use with new exams

    delete res;
}

void DatabaseManager::setLastAnnouncementID() {
    ResultSet *res = executeQuery("SELECT id FROM announcements WHERE id >= ALL(SELECT id FROM announcements);");

    int id = 1;
    if (res->next()) {
        id = res->getInt("id");
    }

    Announcement::setLastID(id + 1);

    delete res;
}

void DatabaseManager::connectToDatabase(string database, string user, string pass, string host) {
    this->database = database;
    this->user = user;
    this->pass = pass;
    this->host = host;

    try {
        this->driver = get_driver_instance();
        delete this->connection;
        this->connection = this->driver->connect(this->host, this->user, this->pass);
        this->connection->setSchema(this->database);
        delete this->stmt;
        this->stmt = this->connection->createStatement();
        setLastExamID();
        setLastAnnouncementID();
    } catch (SQLException &sq) {
        const char *error = sq.what();
        Warning w(error, "DATABASE CONNECTION");
        this->warnings.push_back(w);

        cerr << "Failed to connect to database, see log for details" << endl;
        throw sq;
    }
}

bool DatabaseManager::add(const Lecturer &lecturer)
{
    string query = "INSERT INTO lecturers (email, name, age, department) VALUES ('" + lecturer.getEmail() + "', '" + lecturer.getName() + "', " + std::to_string(lecturer.getAge()) + ", '" + lecturer.getDepartment() + "');";

    try
    {
        this->stmt->execute(query);

        return true; //if you reached this line execute ran without any exceptions, so lecturer was added successfully. This is the same with other add methods
    }
    catch (SQLException &e)
    {
        const char *error = e.what();
        Warning w(error, query);
        this->warnings.push_back(w);

        return false;
    }
}

boost::optional<Lecturer> DatabaseManager::getLecturer(string email)
{
    ResultSet *res = executeQuery("SELECT * FROM lecturers WHERE email = '" + email + "';");

    if (res->next())
    {
        string name = res->getString("name");
        int age = res->getInt("age");
        string department = res->getString("department");

        delete res;

        return Lecturer(name, age, email, department);
    }

    delete res;

    return boost::none;
}

/*bool DatabaseManager::contains(const Lecturer &lecturer) {
    return (bool)getLecturer(lecturer.getID());
}*/

bool DatabaseManager::contains(const DatabaseItem &item)
{
    Tables table = item.getTable();

    switch (table)
    {
    case LECTURERS:
    {
        const Lecturer &lecturer = dynamic_cast<const Lecturer &>(item);
        return (bool)getLecturer(lecturer.getEmail());
    }
    case STUDENTS:
    {
        const Student &student = dynamic_cast<const Student &>(item);
        return (bool)getStudent(student.getID());
    }
    case COURSES:
    {
        const Course &course = dynamic_cast<const Course &>(item);
        return (bool)getCourse(course.getID());
    }
    case MODULES:
    {
        const Module &module = dynamic_cast<const Module &>(item);
        return (bool)getModule(module.getCode());
    }
    case ANNOUNCEMENTS:
    {
        const Announcement &announcement = dynamic_cast<const Announcement &>(item);

        ResultSet *res = executeQuery("SELECT * FROM " + tableNames.at(table) + " WHERE id = " + std::to_string(announcement.getID()) + " AND module = '" + announcement.getModule().getCode() + "';");
        bool containsAnnouncement = res->next();
        delete res;
        return containsAnnouncement;
    }
    case EXAMS:
    {
        const Exam &exam = dynamic_cast<const Exam &>(item);
        return (bool)getExam(exam.getID());
    }
    case EXAM_GRADES:
    {
        const ExamGrade &examGrade = dynamic_cast<const ExamGrade &>(item);
        return (bool)getExamGrade(examGrade.getStudent(), examGrade.getExam());
    }
    case MODULE_GRADES:
    {
        const ModuleGrade &moduleGrade = dynamic_cast<const ModuleGrade &>(item);
        return (bool)getModuleGrade(moduleGrade.getModule(), moduleGrade.getStudent());
    }
    case LECTURER_ACCOUNTS:
    {
        const LecturerAccount &lecturerAccount = dynamic_cast<const LecturerAccount &>(item);
        return (bool)getLecturerAccount(lecturerAccount.getLecturer().getEmail());
    }
    case STUDENT_ACCOUNTS:
    {
        const StudentAccount &studentAccount = dynamic_cast<const StudentAccount &>(item);
        return (bool)getStudentAccount(studentAccount.getStudent().getID());
    }
    case STUDENT_REGISTRATIONS:
    {
        const StudentRegistration &sReg = dynamic_cast<const StudentRegistration &>(item);
        return (bool)getStudentRegistration(sReg.getStudent(), sReg.getModule());
    }
    default:
        return false;
    }
}

vector<Lecturer> DatabaseManager::getAllLecturers()
{
    vector<Lecturer> lecturers;

    ResultSet *res = executeQuery("SELECT id FROM lecturers;");

    while (res->next())
    {
        lecturers.push_back(getLecturer(res->getString("email")).get());
    }

    delete res;

    return lecturers;
}

bool DatabaseManager::remove(const Lecturer &lecturer)
{
    string email = "'" + lecturer.getEmail() + "'";
    bool removed = this->stmt->executeUpdate("DELETE FROM lecturers WHERE email = " + email + ";") != 0;

    return removed;
}

bool DatabaseManager::update(string email, const Lecturer &updatedLecturer)
{
    string email1 = updatedLecturer.getEmail();

    if (email != email1)
    {
        throw KeyMismatch(email, email1);
    }
    else
    {
        string query = "UPDATE lecturers SET name = '" + updatedLecturer.getName() + "', age = " + std::to_string(updatedLecturer.getAge()) + ", department = '" + updatedLecturer.getDepartment() + "' WHERE email = '" + email + "';";

        bool updated = executeUpdate(query) != 0;

        return updated;
    }
}

bool DatabaseManager::add(const Course &course)
{
    string query = "INSERT INTO courses (id, type, name, duration, course_leader) VALUES ('" + course.getID() + "', '" + course.getType() + "', '" + course.getName() + "', " + std::to_string(course.getDuration()) + ", '" + course.getCourseLeader().getEmail() + "');";

    try
    {
        this->stmt->execute(query);

        return true;
    }
    catch (SQLException &e)
    {
        const char *error = e.what();
        Warning w(error, query);
        this->warnings.push_back(w);

        return false;
    }
}

boost::optional<Course> DatabaseManager::getCourse(string id)
{
    ResultSet *res = executeQuery("SELECT * FROM courses WHERE id = '" + id + "';");

    if (res->next())
    {
        string type = res->getString("type");
        string name = res->getString("name");
        int duration = res->getInt("duration");
        boost::optional<Lecturer> lecturer = getLecturer(res->getString("course_leader"));
        delete res;

        return Course(id, type, name, duration, lecturer.get());
    }

    delete res;

    return boost::none;
}

/*bool DatabaseManager::contains(const Course &course) {
    return (bool)getCourse(course.getID());
}*/

vector<Course> DatabaseManager::getAllCourses()
{
    vector<Course> courses;

    ResultSet *res = executeQuery("SELECT id FROM courses;");

    while (res->next())
    {
        courses.push_back(getCourse(res->getString("id")).get());
    }

    delete res;

    return courses;
}

bool DatabaseManager::remove(const Course &course)
{
    string c = "'" + course.getID() + "'";
    bool removed = this->stmt->executeUpdate("DELETE FROM courses WHERE id = " + c + ";") != 0;

    return removed;
}

bool DatabaseManager::update(string id, const Course &updatedCourse)
{
    string id1 = updatedCourse.getID();

    if (id != id1)
    {
        throw KeyMismatch(id, id1);
    }
    else
    {
        string query = "UPDATE courses SET type = '" + updatedCourse.getType() + "', name = '" + updatedCourse.getName() + "', duration = " + std::to_string(updatedCourse.getDuration()) + ", course_leader = '" + updatedCourse.getCourseLeader().getEmail() + "' WHERE id = '" + id1 + "';";

        bool updated = executeUpdate(query) != 0;

        return updated;
    }
}

bool DatabaseManager::add(const Student &student)
{
    string query = "INSERT INTO students (id, name, age, qca, course) VALUES (" + std::to_string(student.getID()) + ", '" + student.getName() + "', " + std::to_string(student.getAge()) + ", " + std::to_string(student.getQCA()) + ", '" + student.getCourse().getID() + "');";

    try
    {
        this->stmt->execute(query);

        return true;
    }
    catch (SQLException &e)
    {
        const char *error = e.what();
        Warning w(error, query);
        this->warnings.push_back(w);

        return false;
    }
}

boost::optional<Student> DatabaseManager::getStudent(int id)
{
    ResultSet *res = executeQuery("SELECT * FROM students WHERE id = " + std::to_string(id) + ";");

    if (res->next())
    {
        string name = res->getString("name");
        int age = res->getInt("age");
        float qca = res->getDouble("qca");
        boost::optional<Course> course = getCourse(res->getString("course"));

        delete res;

        return Student(id, name, age, course.get(), qca);
    }

    delete res;

    return boost::none;
}

/*bool DatabaseManager::contains(const Student &student) {
    return (bool)getStudent(student.getID());
}*/

vector<Student> DatabaseManager::getAllStudents()
{
    vector<Student> students;

    ResultSet *res = executeQuery("SELECT id FROM students;");

    while (res->next())
    {
        students.push_back(getStudent(res->getInt("id")).get());
    }

    delete res;

    return students;
}

bool DatabaseManager::remove(const Student &student)
{
    string id = std::to_string(student.getID());
    bool removed = this->stmt->executeUpdate("DELETE FROM students WHERE id = " + id + ";") != 0;

    return removed;
}

bool DatabaseManager::update(int id, const Student &updatedStudent)
{
    int id1 = updatedStudent.getID();

    if (id != id1)
    {
        throw KeyMismatch(std::to_string(id), std::to_string(id1));
    }
    else
    {
        string query = "UPDATE students SET name = '" + updatedStudent.getName() + "', age = " + std::to_string(updatedStudent.getAge()) + ", qca = " + std::to_string(updatedStudent.getQCA()) + ", course = '" + updatedStudent.getCourse().getID() + "' WHERE id = " + std::to_string(id1) + ";";

        bool updated = executeUpdate(query) != 0;

        return updated;
    }
}

bool DatabaseManager::add(const Module &module)
{
    string query = "INSERT INTO modules (code, name, credits, lecturer) VALUES ('" + module.getCode() + "', '" + module.getName() + "', " + std::to_string(module.getCredits()) + ", '" + module.getLecturer().getEmail() + "');";

    try
    {
        this->stmt->execute(query);

        return true;
    }
    catch (SQLException &e)
    {
        const char *error = e.what();
        Warning w(error, query);
        this->warnings.push_back(w);

        return false;
    }
}

boost::optional<Module> DatabaseManager::getModule(string code)
{
    ResultSet *res = executeQuery("SELECT * FROM modules WHERE code = '" + code + "';");

    if (res->next())
    {
        string name = res->getString("name");
        int credits = res->getInt("credits");
        boost::optional<Lecturer> lecturer = getLecturer(res->getString("lecturer"));

        delete res;

        return Module(code, name, credits, lecturer.get());
    }

    delete res;

    return boost::none;
}

/*bool DatabaseManager::contains(const Module &module) {
    return (bool)getModule(module.getCode());
}*/

vector<Module> DatabaseManager::getAllModules()
{
    vector<Module> modules;

    ResultSet *res = executeQuery("SELECT code FROM modules;");

    while (res->next())
    {
        modules.push_back(getModule(res->getString("code")).get());
    }

    delete res;

    return modules;
}

bool DatabaseManager::remove(const Module &module)
{
    string query = "DELETE FROM modules WHERE code = '" + module.getCode() + "';";

    bool removed = this->stmt->executeUpdate(query) != 0;

    return removed;
}

bool DatabaseManager::update(string code, const Module &updatedModule)
{
    string code1 = updatedModule.getCode();

    if (code != code1)
    {
        throw KeyMismatch(code, code1);
    }
    else
    {
        string query = "UPDATE modules SET name = '" + updatedModule.getName() + "', credits = " + std::to_string(updatedModule.getCredits()) + ", lecturer = '" + updatedModule.getLecturer().getEmail() + "' WHERE code = '" + code1 + "';";

        bool updated = executeUpdate(query) != 0;

        return updated;
    }
}

bool DatabaseManager::add(const Announcement &announcement) {
    string query = "INSERT INTO announcements (id, module, lecturer, subject, announcement) VALUES (?, ?, ?, ?, ?);";

    PreparedStatement *prepared = this->connection->prepareStatement(query);

    try
    {
        prepared->setInt(1, announcement.getID());
        prepared->setString(2, announcement.getModule().getCode());
        prepared->setString(3, announcement.getLecturer().getEmail());
        prepared->setString(4, announcement.getSubject());
        prepared->setString(5, announcement.getAnnouncementText());

        prepared->execute();

        delete prepared;

        return true;
    }
    catch (SQLException &e)
    {
        const char *error = e.what();
        Warning w(error, query);
        this->warnings.push_back(w);

        delete prepared;

        return false;
    }
}

bool DatabaseManager::remove(const Announcement &announcement) {
    string query = "DELETE FROM announcements WHERE id = " + std::to_string(announcement.getID()) + " AND module = '" + announcement.getModule().getCode() + "';";

    bool removed = this->executeUpdate(query) != 0;

    return removed;
}

bool DatabaseManager::update(int id, std::string moduleCode, const Announcement &updatedAnnouncement) {
    string oid = std::to_string(id);
    string nid = std::to_string(updatedAnnouncement.getID());

    string nmoduleCode = updatedAnnouncement.getModule().getCode();

    if (oid != nid && moduleCode != nmoduleCode) {
        throw new KeyMismatch(nid + "-" + nmoduleCode, oid + "-" + moduleCode);
    } else {
        string query = "UPDATE announcements SET lecturer = ?, subject = ?, announcement = ?, time_created = CURRENT_TIMESTAMP() WHERE id = ? AND module = ?;";

        PreparedStatement *prepared = this->connection->prepareStatement(query);

        prepared->setString(1, updatedAnnouncement.getLecturer().getEmail());
        prepared->setString(2, updatedAnnouncement.getSubject());
        prepared->setString(3, updatedAnnouncement.getAnnouncementText());
        prepared->setInt(4, id);
        prepared->setString(5, moduleCode);

        bool updated = prepared->executeUpdate() != 0;

        delete prepared;

        return updated;
    }
}

std::vector<Announcement> DatabaseManager::getAllAnnouncements() {
    std::vector<Announcement> announcements;

    string query = "SELECT * FROM announcements ORDER BY time_created DESC";

    ResultSet *res = executeQuery(query);

    while (res->next()) {
        int id = res->getInt("id");
        string code = res->getString("module");
        string email = res->getString("lecturer");
        string subject = res->getString("subject");
        string text = res->getString("announcement");
        Module module = getModule(code).value();
        Lecturer lecturer = getLecturer(email).value();

        announcements.push_back(Announcement(id, module, lecturer, subject, text));
    }

    delete res;

    return announcements;
}

bool DatabaseManager::add(const StudentRegistration &registration)
{
    string query = "INSERT INTO student_registrations (student, module) VALUES (" + std::to_string(registration.getStudent().getID()) + ", '" + registration.getModule().getCode() + "');";

    try
    {
        this->stmt->execute(query);

        return true;
    }
    catch (SQLException &e)
    {
        const char *error = e.what();
        Warning w(error, query);
        this->warnings.push_back(w);
        
        return false;
    }
}

bool DatabaseManager::remove(const StudentRegistration &registration)
{
    string query = "DELETE FROM student_registrations WHERE student = " + std::to_string(registration.getStudent().getID()) + " AND module = '" + registration.getModule().getCode() + "';";

    bool removed = this->executeUpdate(query) != 0;

    return removed;
}

boost::optional<StudentRegistration> DatabaseManager::getStudentRegistration(const Student &student, const Module &module)
{
    string query = "SELECT * FROM student_registrations WHERE student = " + std::to_string(student.getID()) + " AND module = '" + module.getCode() + "';";

    ResultSet *res = executeQuery(query);

    if (res->next())
    {
        Student s = getStudent(res->getInt("student")).get();
        Module m = getModule(res->getString("module")).get();

        return StudentRegistration(s, m);
    }

    delete res;

    return boost::none;
}

std::vector<StudentRegistration> DatabaseManager::getAllStudentRegistrations()
{
    vector<StudentRegistration> registrations;

    string query = "SELECT * FROM student_registrations;";

    ResultSet *res = executeQuery(query);

    while (res->next())
    {
        registrations.push_back(StudentRegistration(getStudent(res->getInt("student")).get(), getModule(res->getString("module")).get()));
    }

    delete res;

    return registrations;
}

bool DatabaseManager::add(const ExamAnswer &answer)
{
    try
    {
        string query = "INSERT INTO exam_answers (exam, question, answer) VALUES (" + std::to_string(answer.getExamID()) + ", " + std::to_string(answer.getQuestion()) + ", '" + answer.getAnswer() + "');";
        this->stmt->execute(query);

        return true;
    }
    catch (SQLException &e)
    {
        throw e;
    }
}

bool DatabaseManager::add(const ExamQuestion &question)
{
    try
    {
        string query = "INSERT INTO exam_questions (num, exam, question, answer_key, numberOfAnswers) VALUES (" + std::to_string(question.getNumber()) + ", " + std::to_string(question.getExamID()) + ", '" + question.getQuestion() + "', '" + question.getKey().getAnswer() + "', " + std::to_string(question.getNumberOfAnswers()) + ");";
        this->stmt->execute(query);
        bool added = true; //if you reach this line, line above ran ssuccessfully

        for (const ExamAnswer &examAnswer : question.getPossibleAnswers())
        {
            bool answerAdded = add(examAnswer);
            added = added && answerAdded;
        }

        return added;
    }
    catch (SQLException &e)
    {
        throw e;
    }
}

//CHECK SCHEMA TO SEE IF THE TABLES WORK I.E CLEAR EVERY TABLE AND REIMPORT TO CHECK FOR SYNTAX ERRORS
//this may not work, you'll have to edit getExam too but remove may be fine because you have cascades
bool DatabaseManager::add(const Exam &exam)
{
    string query = "INSERT INTO exams (id, module, name, year, semester, numberOfQuestions, weightPerQuestion, totalWeight) VALUES (" + std::to_string(exam.getID()) + ", '" + exam.getModule().getCode() + "', '" + exam.getName() + "', " + std::to_string(exam.getYear()) + ", " + std::to_string(exam.getSemester()) + ", " + std::to_string(exam.getNumberOfQuestions()) + ", " + std::to_string(exam.getWeightPerQuestion()) + ", " + std::to_string(exam.getTotalWeight()) + ");";

    try
    {
        this->stmt->execute(query);
        bool added = true;

        for (const ExamQuestion &examQuestion : exam.getQuestions())
        {
            bool questionAdded = add(examQuestion);
            added = added && questionAdded;
        }

        return added;
    }
    catch (SQLException &e)
    {
        const char *error = e.what();
        Warning w(error, query);
        this->warnings.push_back(w);

        return false;
    }
}
//Gets the position of the minimum element in the vector
int minPosition(int startPos, const std::vector<ExamQuestion> &vect)
{
    int min = startPos;

    for (int i = startPos; i < vect.size(); i++)
    {
        if (vect[i] < vect[min])
        {
            min = i;
        }
    }

    return min;
}

void sort(std::vector<ExamQuestion> &vect)
{
    int n = vect.size();
    for (int i = 0; i < n - 1; i++)
    {
        int min = minPosition(i, vect); //find the position of the smallest item from the start position i, and this is the item you will want to swap

        ExamQuestion temp(vect[i]);
        vect[i] = vect[min];
        vect[min] = temp;
    }
}

vector<ExamQuestion> DatabaseManager::getAllExamQuestions(int examID)
{
    vector<ExamQuestion> questions;

    ResultSet *res = executeQuery("SELECT * FROM exam_questions WHERE exam = " + std::to_string(examID) + ";");

    while (res->next())
    {
        int number = res->getInt("num");
        string question = res->getString("question");
        string answer_key = res->getString("answer_key");
        int numberOfAnswers = res->getInt("numberOfAnswers");

        vector<ExamAnswer> answers;

        ResultSet *res1 = executeQuery("SELECT * FROM exam_answers WHERE exam = " + std::to_string(examID) + " AND question = " + std::to_string(number) + ";");

        while (res1->next())
        {
            string answer = res1->getString("answer");

            answers.push_back(ExamAnswer(examID, number, answer));
        }

        ExamQuestion examQuestion(examID, number, question, ExamAnswer(examID, number, answer_key, true), answers, numberOfAnswers);
        questions.push_back(examQuestion);

        delete res1;
    }

    sort(questions);

    delete res;

    return questions;
}

boost::optional<Exam> DatabaseManager::getExam(int id)
{
    ResultSet *res = executeQuery("SELECT * FROM exams WHERE id = " + std::to_string(id) + ";");

    if (res->next())
    {
        boost::optional<Module> module = getModule(res->getString("module"));
        string name = res->getString("name");
        int year = res->getInt("year");
        int semester = res->getInt("semester");
        int numQuestions = res->getInt("numberOfQuestions");
        float totalW = res->getDouble("totalWeight");

        vector<ExamQuestion> questions = this->getAllExamQuestions(id);

        delete res;

        return Exam(id, module.get(), name, year, semester, numQuestions, totalW, questions);
    }

    delete res;

    return boost::none;
}

/*bool DatabaseManager::contains(const Exam &exam) {
    return (bool)getExam(exam.getID());
}*/

vector<Exam> DatabaseManager::getAllExams()
{
    vector<Exam> exams;

    ResultSet *res = executeQuery("SELECT id FROM exams;");

    while (res->next())
    {
        exams.push_back(getExam(res->getInt("id")).get());
    }

    delete res;

    return exams;
}

bool DatabaseManager::remove(const Exam &exam)
{
    string module = "'" + exam.getModule().getCode() + "'";
    string name = "'" + exam.getName() + "'";
    string numQuestions = std::to_string(exam.getNumberOfQuestions());
    string weight = std::to_string(exam.getWeightPerQuestion());
    string totalW = std::to_string(exam.getTotalWeight()); //most likely to be only one result if all attributes match

    bool deleted = this->stmt->executeUpdate("DELETE FROM exams WHERE module = " + module + " AND " + "name = " + name + " AND " + "numberOfQuestions = " + numQuestions + " AND " + "weightPerQuestion = " + weight + " AND " + "totalWeight = " + totalW + ";") != 0;

    return deleted;
}

//this may not be correct
bool DatabaseManager::update(const ExamAnswer &oldAnswer, const ExamAnswer &newAnswer)
{
    string oid = std::to_string(oldAnswer.getExamID());
    string nid = std::to_string(newAnswer.getExamID());

    string query = "UPDATE exam_answers SET answer = '" + newAnswer.getAnswer() + "', question = " + std::to_string(newAnswer.getQuestion()) + " WHERE exam = " + oid + " AND question = " + std::to_string(oldAnswer.getQuestion()) + " AND answer = '" + oldAnswer.getAnswer() + "';";

    return executeUpdate(query) != 0;
}

bool DatabaseManager::update(const ExamQuestion &oldQuestion, const ExamQuestion &newQuestion)
{
    string oid = std::to_string(oldQuestion.getExamID());
    string nid = std::to_string(newQuestion.getExamID());
    bool updated = true;

    string query = "UPDATE exam_questions SET exam = " + nid + ", question = '" + newQuestion.getQuestion() + "', answer_key = '" + newQuestion.getKey().getAnswer() + "', numberOfAnswers = " + std::to_string(newQuestion.getNumberOfAnswers()) + " WHERE exam = " + oid + " AND num = '" + std::to_string(oldQuestion.getNumber()) + "';";

    //executeUpdate will always be called since updated is always true when this line is reached
    updated = updated && executeUpdate(query) != 0;

    vector<ExamAnswer> oldAnswers = oldQuestion.getPossibleAnswers();
    vector<ExamAnswer> newAnswers = newQuestion.getPossibleAnswers();

    int osize = oldAnswers.size();

    for (int i = 0; i < osize; i++)
    {
        ExamAnswer oldAnswer = oldAnswers[i];
        ExamAnswer newAnswer = newAnswers[i];
        bool answersUpdated = update(oldAnswer, newAnswer);
        updated = updated || answersUpdated;
    }

    return updated;
}

bool DatabaseManager::update(const Exam &oldExam, const Exam &updatedExam)
{
    int id = oldExam.getID();
    int id1 = updatedExam.getID();

    if (id != id1)
    {
        throw KeyMismatch(std::to_string(id), std::to_string(id1));
    }
    else
    {
        string query = "UPDATE exams SET module = '" + updatedExam.getModule().getCode() + "', name = '" + updatedExam.getName() + "', year = " + std::to_string(updatedExam.getYear()) + ", semester = " + std::to_string(updatedExam.getSemester()) + ", numberOfQuestions = " + std::to_string(updatedExam.getNumberOfQuestions()) + ", weightPerQuestion = " + std::to_string(updatedExam.getWeightPerQuestion()) + ", totalWeight = " + std::to_string(updatedExam.getTotalWeight()) + " WHERE id = " + std::to_string(id) + ";";

        bool updated = executeUpdate(query) != 0;

        vector<ExamQuestion> oldQuestions = oldExam.getQuestions();
        vector<ExamQuestion> newQuestions = updatedExam.getQuestions();

        for (int i = 0; i < newQuestions.size(); i++)
        {
            ExamQuestion oldQ = oldQuestions[i];
            ExamQuestion newQ = newQuestions[i];
            updated = updated || update(oldQ, newQ);
        }

        return updated;
    }
}

bool DatabaseManager::add(const ExamGrade &examGrade)
{
    Exam exam = examGrade.getExam();
    int id = exam.getID();
    string query = "INSERT INTO exam_grades (student, exam, grade) VALUES (" + std::to_string(examGrade.getStudent().getID()) + ", " + std::to_string(id) + ", " + std::to_string(examGrade.getGrade()) + ");";

    try
    {
        this->stmt->execute(query);

        return true;
    }
    catch (SQLException &e)
    {
        const char *error = e.what();
        Warning w(error, query);
        this->warnings.push_back(w);
    
        return false;
    }
}

boost::optional<ExamGrade> DatabaseManager::getExamGrade(const Student &student, const Exam &exam)
{
    ResultSet *res = executeQuery("SELECT * FROM exam_grades WHERE student = " + std::to_string(student.getID()) + " AND exam = " + std::to_string(exam.getID()) + ";");

    if (res->next())
    {
        float grade = res->getDouble("grade");

        delete res;

        return ExamGrade(student, exam, grade);
    }

    delete res;

    return boost::none;
}

/*bool DatabaseManager::contains(const ExamGrade &examGrade) {
    return (bool)getExamGrade(examGrade.getStudent(), examGrade.getExam());
}*/

vector<ExamGrade> DatabaseManager::getAllExamGrades()
{
    vector<ExamGrade> examGrades;

    ResultSet *res = executeQuery("SELECT student, exam FROM exam_grades;");

    while (res->next())
    {
        Student student = getStudent(res->getInt("student")).get();
        Exam exam = getExam(res->getInt("exam")).get();
        examGrades.push_back(getExamGrade(student, exam).get());
    }

    delete res;

    return examGrades;
}

bool DatabaseManager::remove(const ExamGrade &examGrade)
{
    string query = "DELETE FROM exam_grades WHERE student = " + std::to_string(examGrade.getStudent().getID()) + " AND exam = " + std::to_string(examGrade.getExam().getID()) + ";";
    bool removed = this->stmt->executeUpdate(query) != 0;

    return removed;
}

bool DatabaseManager::update(const Student &student, const Exam &exam, const ExamGrade &updatedExamGrade)
{
    int sId = student.getID();
    int sId1 = updatedExamGrade.getStudent().getID();
    int eId = exam.getID();
    int eId1 = updatedExamGrade.getStudent().getID();

    if (sId != sId1 && eId != eId1)
    {
        string key = std::to_string(sId) + "-" + std::to_string(eId);
        string key1 = std::to_string(sId1) + "-" + std::to_string(eId1);
        throw KeyMismatch(key, key1);
    }
    else
    {
        string query = "UPDATE exam_grades SET grade = " + std::to_string(updatedExamGrade.getGrade()) + " WHERE student = " + std::to_string(sId1) + " AND exam = " + std::to_string(eId1) + ";";

        bool updated = executeUpdate(query) != 0;

        return updated;
    }
}

void DatabaseManager::calculateModuleGrades(std::string module, const Student &student)
{
    string query = "CALL calculate_grades('" + module + "', " + std::to_string(student.getID()) + ");";

    try
    {
        this->stmt->execute(query);
    }
    catch (SQLException &e)
    {
        const char *error = e.what();
        Warning w(error, query);
        this->warnings.push_back(w);
    }
}

boost::optional<ModuleGrade> DatabaseManager::getModuleGrade(const Module &module, const Student &student)
{
    string moduleCode = "'" + module.getCode() + "'";
    int id = student.getID();

    ResultSet *res = this->stmt->executeQuery("SELECT * FROM module_grades WHERE module = " + moduleCode + " AND student = " + std::to_string(id) + ";");

    if (res->next())
    {
        boost::optional<Module> module = getModule(res->getString("module"));
        boost::optional<Student> student = getStudent(res->getInt("student"));
        float mark = res->getDouble("mark");

        delete res;

        return ModuleGrade(module.get(), student.get(), mark);
    }

    delete res;

    return boost::none;
}

vector<ModuleGrade> DatabaseManager::getAllModuleGrades()
{
    vector<ModuleGrade> moduleGrades;

    ResultSet *res = executeQuery("SELECT module, student FROM module_grades;");

    while (res->next())
    {
        Module module = getModule(res->getString("module")).get();
        Student student = getStudent(res->getInt("student")).get();
        moduleGrades.push_back(getModuleGrade(module, student).get());
    }

    delete res;

    return moduleGrades;
}

bool DatabaseManager::add(const LecturerAccount &lecturerAccount)
{
    string query = "INSERT INTO lecturer_accounts (email, pass) VALUES ('" + lecturerAccount.getEmail() + "', '" + lecturerAccount.getPassword() + "');";

    try
    {
        this->stmt->execute(query);

        return true;
    }
    catch (SQLException &e)
    {
        const char *error = e.what();
        Warning w(error, query);
        this->warnings.push_back(w);

        return false;
    }
}

boost::optional<LecturerAccount> DatabaseManager::getLecturerAccount(string email)
{
    string query = "SELECT * FROM lecturer_accounts WHERE email = '" + email + "';";

    ResultSet *res = executeQuery(query);

    if (res->next())
    {
        boost::optional<Lecturer> lecturer = getLecturer(email);
        string pass = res->getString("pass");

        delete res;

        return LecturerAccount(lecturer.get(), pass);
    }

    delete res;

    return boost::none;
}

vector<LecturerAccount> DatabaseManager::getAllLecturerAccounts()
{
    vector<LecturerAccount> accounts;

    ResultSet *res = executeQuery("SELECT id FROM lecturer_accounts;");

    while (res->next())
    {
        accounts.push_back(getLecturerAccount(res->getString("email")).get());
    }

    delete res;

    return accounts;
}

bool DatabaseManager::remove(const LecturerAccount &lecturerAccount)
{
    bool removed = executeUpdate("DELETE FROM lecturer_accounts WHERE email = '" + lecturerAccount.getEmail() + "';") != 0;

    return removed;
}

bool DatabaseManager::update(const Lecturer &lecturer, const LecturerAccount &updatedLecturerAccount)
{
    string email = lecturer.getEmail();
    string email1 = updatedLecturerAccount.getLecturer().getEmail();

    if (email != email1)
    {
        throw KeyMismatch(email, email1);
    }
    else
    {
        string query = "UPDATE lecturer_accounts SET pass = '" + updatedLecturerAccount.getPassword() + "' WHERE email = '" + email + "';";

        bool updated = executeUpdate(query) != 0;

        return updated;
    }
}

bool DatabaseManager::add(const StudentAccount &studentAccount)
{
    string query = "INSERT INTO student_accounts (id, pass) VALUES (" + std::to_string(studentAccount.getStudent().getID()) + ", '" + studentAccount.getPassword() + "');";

    try
    {
        this->stmt->execute(query);

        return true;
    }
    catch (SQLException &e)
    {
        const char *error = e.what();
        Warning w(error, query);
        this->warnings.push_back(w);
        
        return false;
    }
}

boost::optional<StudentAccount> DatabaseManager::getStudentAccount(int id)
{
    string query = "SELECT * FROM student_accounts WHERE id = " + std::to_string(id) + ";";

    ResultSet *res = executeQuery(query);

    if (res->next())
    {
        boost::optional<Student> student = getStudent(id);
        string pass = res->getString("pass");

        delete res;

        return StudentAccount(student.get(), pass);
    }

    delete res;

    return boost::none;
}

vector<StudentAccount> DatabaseManager::getAllStudentAccounts()
{
    vector<StudentAccount> accounts;

    ResultSet *res = executeQuery("SELECT id FROM student_accounts;");

    while (res->next())
    {
        accounts.push_back(getStudentAccount(res->getInt("id")).get());
    }

    delete res;

    return accounts;
}

bool DatabaseManager::remove(const StudentAccount &studentAccount)
{
    bool removed = executeUpdate("DELETE FROM student_accounts WHERE id = " + std::to_string(studentAccount.getStudent().getID()) + ";") != 0;

    return removed;
}

bool DatabaseManager::update(const Student &student, const StudentAccount &updatedStudentAccount)
{
    int id = student.getID();
    int id1 = updatedStudentAccount.getStudent().getID();

    if (id != id1)
    {
        throw KeyMismatch(std::to_string(id), std::to_string(id1));
    }
    else
    {
        string query = "UPDATE student_accounts SET pass = '" + updatedStudentAccount.getPassword() + "' WHERE id = " + std::to_string(id1) + ";";

        bool updated = executeUpdate(query) != 0;

        return updated;
    }
}

void DatabaseManager::execute(string query)
{
    try
    {
        this->stmt->execute(query);
    }
    catch (SQLException &e)
    {
        const char *error = e.what();
        Warning w(error, query);
        this->warnings.push_back(w);
    }
}

ResultSet *DatabaseManager::executeQuery(string query)
{
    ResultSet *res = this->stmt->executeQuery(query);

    return res;
}

int DatabaseManager::executeUpdate(string query)
{
    return this->stmt->executeUpdate(query);
}

void DatabaseManager::clearTable(Tables table)
{
    this->stmt->executeUpdate("TRUNCATE TABLE IF EXISTS " + tableNames.at(table) + ";");
}

void DatabaseManager::clearDatabase()
{
    this->stmt->execute("SET foreign_key_checks = 0;");
    for (int i = 0; i < MAX; i++)
    {
        clearTable(static_cast<Tables>(i));
    }
    this->stmt->execute("SET foreign_key_checks = 1;");

    setLastExamID();
}

vector<Warning> DatabaseManager::getWarnings() const
{
    return this->warnings;
}

void DatabaseManager::clearWarnings()
{
    this->warnings.clear();
}

void DatabaseManager::writeWarningsToLog()
{
    if (warnings.size() == 0)
        return;

    time_t now = time(0);

    tm *ltm = localtime(&now);

    stringstream s;
    const char* logDir = std::getenv("STUD_LOGS");
    if (logDir) {
        s << logDir << "/" << ltm->tm_mday << "_" << ltm->tm_mon + 1 << "_" << 1900 + ltm->tm_year << "_" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "_Database_Warnings.log";
        string filename;
        s >> filename;
        ofstream writer;
        writer.open(filename, std::ios::out);

        if (writer.is_open())
        {
            for (const Warning &w : warnings)
            {
                writer << w.toString() << "\n";
            }

            if (this->connection) {
                const SQLWarning *warning = this->connection->getWarnings();

                while (warning != NULL) {
                    Warning w(warning->getMessage(), "N/A");
                    writer << w.toString() << "\n";
                    warning = warning->getNextWarning();
                }
            }

            writer.flush();
        }
    }
}

DatabaseManager &DatabaseManager::operator=(const DatabaseManager &manager) {
    /*this->driver = get_driver_instance();
    this->connection = this->driver->connect(manager.host, manager.user, manager.pass);
    this->connection->setSchema(manager.database);
    this->stmt = this->connection->createStatement();
*/

    this->host = manager.host;
    this->user = manager.user;
    this->pass = manager.pass;
    this->database = manager.database;

    this->connectToDatabase(this->host, this->user, this->pass, this->database);

    setLastExamID();
    
    return *this;
}