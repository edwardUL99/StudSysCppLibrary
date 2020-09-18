#include "headers/StudentSystem.h"
#include "headers/DuplicateException.h"
#include "headers/NotFoundException.h"
#include "headers/KeyMismatch.h"
#include "headers/StudentRegistration.h"
#include "headers/Logging.h"

using std::string;
using logging::logger;
using logging::LogTypes;

StudentSystem::StudentSystem() {}

StudentSystem::StudentSystem(const StudentSystem &system) {
    this->database = DatabaseManager(system.database);
}

void StudentSystem::logInfo(string message) {
    if (database.logging) {
        time_t ttime = time(0);
        logger.appendToLogFile(database.logFileName, LogTypes::INFO, "StudentSystem: " + message + " at " + ctime(&ttime));
    }
}

void StudentSystem::logError(string message) {
    if (database.logging) {
        time_t ttime = time(0);
        logger.appendToLogFile(database.logFileName, LogTypes::ERROR, "StudentSystem: " + message + " at " + ctime(&ttime));
    }
}

void StudentSystem::logWarning(string message) {
    if (database.logging) {
        time_t ttime = time(0);
        logger.appendToLogFile(database.logFileName, LogTypes::WARNING, "StudentSystem: " + message + " at " + ctime(&ttime));
    }
}

void StudentSystem::startSystem(string database, string user, string pass, string host) {
    logInfo("Starting system and requesting the Database Manager to connect to the database");
    this->database.connectToDatabase(database, user, pass, host);
}

string StudentSystem::recordLogin(const Account &account) {
    logInfo("Attempting to record login for Account " + account.getName());
    string date = "FIRST_LOGIN";

    ResultSet *res = NULL;

    try {
        const StudentAccount &student = dynamic_cast<const StudentAccount&>(account);
        logInfo("Account is a Student Account:\n\t" + student.getDescription());
        string id = std::to_string(student.getStudent().getID());

        res = this->database.executeQuery("SELECT loginTime FROM student_logins WHERE loginTime >= ALL(SELECT loginTime FROM student_logins) AND id = " + id + ";");

        if (res->next()) {
            date = res->getString("loginTime");
        }

        this->database.execute("INSERT INTO student_logins (id) VALUES (" + id + ");");
    } catch (std::bad_cast &b) {}

    try {
        const LecturerAccount &lecturer = dynamic_cast<const LecturerAccount&>(account);
        logInfo("Account is a LecturerAccount:\n\t" + lecturer.getDescription());
        string email = "'" + lecturer.getEmail() + "'";

        res = this->database.executeQuery("SELECT loginTime FROM lecturer_logins WHERE loginTime >= ALL(SELECT loginTime FROM lecturer_logins) AND email = " + email + ";");

        if (res->next()) {
            date = res->getString("loginTime");
        }

        this->database.execute("INSERT INTO lecturer_logins (email) VALUES (" + email + ");");
    } catch (std::bad_cast &b) {}

    delete res;

    return date;
}

bool StudentSystem::addLecturer(const Lecturer &lecturer) {
    logInfo("Request to add Lecturer: " + lecturer.getDescription());
    if (this->database.contains(lecturer)) {
        logError("Lecturer already exists, aborting adding to system");
        throw DuplicateException(lecturer.getDescription());
    } else {
        logInfo("Lecturer does not exist, adding to system");
        return this->database.add(lecturer);
    }
}

Lecturer StudentSystem::getLecturer(string email) {
    logInfo("Request to retrieve Lecturer with e-mail: " + email + " from system");
    boost::optional<Lecturer> lecturer = this->database.getLecturer(email);

    if (lecturer) {
        logInfo("Lecturer found");
        return lecturer.get();
    } else {
        logError("Lecturer not found in the system");
        throw NotFoundException("Lecturer identified by " + email);
    }
}

bool StudentSystem::removeLecturer(const Lecturer &lecturer) {
    logInfo("Request to delete lecturer " + lecturer.getDescription() + " from the system");
    return this->database.remove(lecturer);
}

bool StudentSystem::updateLecturer(string email, const Lecturer &updatedLecturer) {
    logInfo("Attempting to update lecturer " + email);
    return this->database.update(email, updatedLecturer);
}

bool StudentSystem::addStudent(const Student &student) {
    logInfo("Request to add student: " + student.getDescription());
    if (this->database.contains(student)) {
        logError("System already contains the student, aborting");
        throw DuplicateException(student.getDescription());
    } else {
        logInfo("System does not contain the student, adding");
        return this->database.add(student);
    }
}

Student StudentSystem::getStudent(int id) {
    logInfo("Attempting to retrieve student with ID " + std::to_string(id) + " from the system");
    boost::optional<Student> student = this->database.getStudent(id);

    if (student) {
        logInfo("Student found");
        return student.get();
    } else {
        logError("Student not found in the system");
        throw NotFoundException("Student identified by " + std::to_string(id));
    }
}

bool StudentSystem::removeStudent(const Student &student) {
    logInfo("Removing student " + student.getDescription() + " from the system");
    return this->database.remove(student);
}

bool StudentSystem::updateStudent(int id, const Student &updatedStudent) {
    try {
        logInfo("Attempting to update student with ID " + std::to_string(id));
        return this->database.update(id, updatedStudent);
    } catch (KeyMismatch &m) {
        logError("TFailed to update student as there was a Key Mismatch " + string(m.what()));
        throw m;
    }
}

int StudentSystem::getStudentID(string email) {
    logInfo("Attempting to map student email " + email + " to ID");
    for (const Student &s : this->database.getAllStudents()) {
        if (s.getEmail() == email) {
            logInfo("ID found for student");
            return s.getID();
        }
    }

    logWarning("No ID found, so returning -1");

    return -1;
}

bool StudentSystem::addCourse(const Course &course) {
    logInfo("Request to add course " + course.getDescription());
    if (this->database.contains(course)) {
        logError("Course already exists in the system, aborting");
        throw DuplicateException(course.getDescription());
    } else {
        logInfo("System does not contain the course, adding");
        return this->database.add(course);
    }
}

Course StudentSystem::getCourse(string code) {
    logInfo("Attempting to retrieve course with code " + code + " from system");
    boost::optional<Course> course = this->database.getCourse(code);

    if (course) {
        logInfo("Course found");
        return course.get();
    } else {
        logError("Course not found in system");
        throw NotFoundException("Course identified by " + code);
    }
}

bool StudentSystem::removeCourse(const Course &course) {
    logInfo("Request to remove course " + course.getDescription());
    return this->database.remove(course);
}

bool StudentSystem::updateCourse(string id, const Course &updatedCourse) {
    try {
        logInfo("Attempting to update course with ID " + id);
        return this->database.update(id, updatedCourse);
    } catch (KeyMismatch &m) {
        logError("Failed to update course as there was a Key Mismatch " + string(m.what()));
        throw m;
    }
}

bool StudentSystem::addModule(const Module &module) {
    logInfo("Request to add Module " + module.getDescription() + " to system");
    if (this->database.contains(module)) {
        logError("Module already exists in the system, aborting");
        throw DuplicateException(module.getDescription());
    } else {
        logInfo("System does not contain the module, adding");
        return this->database.add(module);
    }
}

Module StudentSystem::getModule(string code) {
    logInfo("Attempting to retrieve Module with code " + code + " from the system");
    boost::optional<Module> module = this->database.getModule(code);

    if (module) {
        logInfo("The module was found");
        return module.get();
    } else {
        logError("The module was not found in the system");
        throw NotFoundException("Module identified by " + code);
    }
}

bool StudentSystem::removeModule(const Module &module) {
    logInfo("Request to remove module from the system " + module.getDescription());
    return this->database.remove(module);
}

bool StudentSystem::updateModule(string code, const Module &updatedModule) {
    try {
        logInfo("Attempting to update module with code " + code);
        return this->database.update(code, updatedModule);
    } catch (KeyMismatch &m) {
        logError("Failed to update module as Key Mismatch occurred " + string(m.what()));
        throw m;
    }
}

std::vector<Module> StudentSystem::getModules() {
    logInfo("Retrieving all modules from the System");
    return this->database.getAllModules();
}

bool StudentSystem::addAnnouncement(const Announcement &announcement) {
    logInfo("Request to add Announcement " + announcement.getDescription() + " to system");
    if (this->database.contains(announcement)) {
        logError("Announcement already exists in the system, aborting");
        throw DuplicateException(announcement.getDescription());
    } else {
        logInfo("System does not contain the announcement, adding");
        return this->database.add(announcement);
    }
}

bool StudentSystem::removeAnnouncement(const Announcement &announcement) {
    logInfo("Request to remove module from the system " + announcement.getDescription());
    return this->database.remove(announcement);
}

std::vector<Announcement> StudentSystem::getModuleAnnouncements(const Module &module) {
    logInfo("Request to retrieve all announcements for Module " + module.getCode());
    std::vector<Announcement> moduleAnnouncements;
    std::vector<Announcement> allAnnouncements = this->database.getAllAnnouncements();

    for (const Announcement &announcement : allAnnouncements) {
        if (announcement.getModule().getCode() == module.getCode()) {
            moduleAnnouncements.push_back(announcement);
        }
    }

    return moduleAnnouncements;
}

bool StudentSystem::updateAnnouncement(int announcementNumber, std::string moduleCode, const Announcement &updatedAnnouncement) {
    try {
        logInfo("Attempting to update announcement " + std::to_string(announcementNumber));
        return this->database.update(announcementNumber, moduleCode, updatedAnnouncement);
    } catch (KeyMismatch &m) {
        logError("Failed to update announcement as Key Mismatch occurred " + string(m.what()));
        throw m;
    }
}

string StudentSystem::getAnnouncementTime(const Announcement &announcement) {
    logInfo("Retrieving time last modified for announcement " + std::to_string(announcement.getID()));
    ResultSet *res = this->database.executeQuery("SELECT time_created FROM announcements WHERE id = " + std::to_string(announcement.getID()) + " AND module = '" + announcement.getModule().getCode() + "';");

    string time = "";
    if (res->next()) {
        time = res->getString("time_created");
    }

    delete res;

    return time;
}

bool StudentSystem::registerStudentModule(const Student &student, const Module &module) {
    logInfo("Request to register Student " + student.getDescription() + " on Module " + module.getDescription());
    StudentRegistration registration(student, module);

    if (this->database.contains(registration)) {
        logError("System already contains a registration for this student, aborting");
        throw DuplicateException(registration.getDescription());
    } else {
        logInfo("System does not contain a registration for this student, adding");
        return this->database.add(registration);
    }
}

bool StudentSystem::unregisterStudentModule(const Student &student, const Module &module) {
    logInfo("Request to remove registration for Student " + student.getDescription() + " from Module " + module.getDescription());
    StudentRegistration registration(student, module);

    return this->database.remove(registration);
}

std::vector<Module> StudentSystem::getStudentRegisteredModules(const Student &student) {
    logInfo("Retrieving all modules that Student " + student.getDescription() + " is registered for");
    return database.getAllModulesStudentIsRegisteredOn(student);
}

std::vector<Student> StudentSystem::getStudentsRegisteredOnModule(const Module &module) {
    logInfo("Retrieving all Students registered on Module " + module.getDescription());
    return database.getAllStudentsRegisteredOnModule(module);
}

bool StudentSystem::addExam(const Exam &exam) {
    logInfo("Request to add Exam " + exam.getDescription() + " to system");
    if (this->database.contains(exam)) {
        logError("Exam already exists in the system, aborting");
        throw DuplicateException(exam.getDescription());
    } else {
        logInfo("System does not contain the exam, adding");
        return this->database.add(exam);
    }
}

Exam StudentSystem::getExam(int id) {
    logInfo("Retrieving exam with ID " + std::to_string(id));
    boost::optional<Exam> exam = this->database.getExam(id);

    if (exam) {
        logInfo("Exam found");
        return exam.get();
    } else {
        logInfo("Exam not found in the system");
        throw NotFoundException("Exam identified by " + std::to_string(id));
    }
}

bool StudentSystem::removeExam(const Exam &exam) {
    logInfo("Request to remove exam from the system " + exam.getDescription());
    return this->database.remove(exam);
}

bool StudentSystem::updateExam(const Exam &oldExam, const Exam &updatedExam) {
    try {
        logInfo("Attempting to update Exam with ID " + std::to_string(oldExam.getID()));
        return this->database.update(oldExam, updatedExam);
    } catch (KeyMismatch &m) {
        logError("Failed to update exam as Key Mismatch occurred " + string(m.what()));
        throw m;
    }
}

std::vector<Exam> StudentSystem::retrieveExamsByModule(const Module &module) {
    logInfo("Retrieving all exams for Module " + module.getCode());
    return database.getAllExamsByModule(module);
}

bool StudentSystem::examTaken(const Exam &exam) {
    logInfo("Checking if Exam " + exam.getDescription() + " has been taken by Students");
    ResultSet *res = this->database.executeQuery("SELECT * FROM exam_grades WHERE exam = " + std::to_string(exam.getID()) + ";");
    bool taken = res->next(); //if res has an item from next, the exam has been taken

    delete res;
    return taken;
}

bool StudentSystem::addExamGrade(const ExamGrade &examGrade) {
    logInfo("Request to add Exam Grade " + examGrade.getDescription() + " to system");
    if (this->database.contains(examGrade)) {
        logError("Exam Grade already exists in the system, aborting");
        throw DuplicateException(examGrade.getDescription());
    } else {
        logInfo("System does not contain the Exam Grade, adding");
        return this->database.add(examGrade);
    }
}

ExamGrade StudentSystem::getExamGrade(const Student &student, const Exam &exam) {
    logInfo("Retrieving Exam Grade for Student " + student.getDescription() + " for Exam " + exam.getDescription());
    boost::optional<ExamGrade> examGrade = this->database.getExamGrade(student, exam);

    if (examGrade) {
        logInfo("Exam Grade found");
        return examGrade.get();
    } else {
        logError("Exam Grade not found in the system");
        throw NotFoundException("Exam Grade identified by Student " + std::to_string(student.getID()) + " and Exam " + std::to_string(exam.getID()));
    }
}

bool StudentSystem::removeExamGrade(const ExamGrade &examGrade) {
    logInfo("Removing Exam Grade " + examGrade.getDescription() + " from system");
    return this->database.remove(examGrade);
}

bool StudentSystem::updateExamGrade(const Student &student, const Exam &exam, const ExamGrade &updatedExamGrade) {
    try {
        logInfo("Attempting to update Exam Grade with Student " + student.getDescription() + " and Exam " + exam.getDescription());
        return this->database.update(student, exam, updatedExamGrade);
    } catch (KeyMismatch &m) {
        logError("Failed to update Exam Grade as Key Mismatch occurred " + string(m.what()));
        throw m;
    }
}

ModuleGrade StudentSystem::getModuleGrade(const Module &module, const Student &student) {
    logInfo("Retrieving Module Grade for Module " + module.getDescription() + " and Student " + student.getDescription());
    boost::optional<ModuleGrade> moduleGrade = this->database.getModuleGrade(module, student);

    if (moduleGrade) {
        logInfo("Module Grade found");
        return moduleGrade.get();
    } else {
        logError("Module Grade not found in system");
        throw NotFoundException("Module Grade identified by module " + module.getCode() + " and student " + std::to_string(student.getID()));
    }
}

void StudentSystem::calculateModuleGrade(string module, const Student &student) {
    logInfo("Calculating Module Grade for Student " + student.getDescription() + " on Module " + module);
    this->database.calculateModuleGrades(module, student);
}

void StudentSystem::calculateAllModuleGradesForModule(string module) {
    logInfo("Calculating all Module Grades for every student in Module " + module);

    for (const Student &student : getStudentsRegisteredOnModule(getModule(module))) {
        calculateModuleGrade(module, student);
    }
}

bool StudentSystem::addAccount(LecturerAccount lecturerAccount) {
    logInfo("Request to add Lecturer Account " + lecturerAccount.getDescription() + " to system");
    if (this->database.contains(lecturerAccount)) {
        logError("Lecturer Account already exists in the system, aborting");
        throw DuplicateException(lecturerAccount.getDescription());
    } else {
        logInfo("System does not contain the Lecturer Account, adding");
        return this->database.add(lecturerAccount);
    }
}

LecturerAccount StudentSystem::getLecturerAccount(string email) {
    logInfo("Retrieving Lecturer Account matching e-mail " + email);
    boost::optional<LecturerAccount> lecturerAccount = this->database.getLecturerAccount(email);

    if (lecturerAccount) {
        logInfo("Lecturer Account found");
        return lecturerAccount.get();
    } else {
        logError("Lecturer Account not found in system");
        throw NotFoundException("Lecturer Account identified by lecturer " + email);
    }
}

bool StudentSystem::removeAccount(const LecturerAccount &lecturerAccount) {
    logInfo("Request to remove Lecturer Account " + lecturerAccount.getDescription() + " from system");
    return this->database.remove(lecturerAccount);
}

bool StudentSystem::updateAccount(const Lecturer &lecturer, const LecturerAccount &updatedLecturerAccount) {
    try {
        logInfo("Attempting to update Lecturer Account for Lecturer " + lecturer.getDescription());
        return this->database.update(lecturer, updatedLecturerAccount);
    } catch (KeyMismatch &m) {
        logError("Failed to update Lecturer Account as Key Mismatch occurred " + string(m.what()));
        throw m;
    }
}

bool StudentSystem::addAccount(const StudentAccount &studentAccount) {
    logInfo("Request to add Student Account " + studentAccount.getDescription() + " to system");
    if (this->database.contains(studentAccount)) {
        logError("Student Account already exists in the system, aborting");
        throw DuplicateException(studentAccount.getDescription());
    } else {
        logInfo("System does not contain the Student Account, adding");
        return this->database.add(studentAccount);
    }
}

StudentAccount StudentSystem::getStudentAccount(int id) {
    logInfo("Retrieving Student Account matching ID " + std::to_string(id));
    boost::optional<StudentAccount> studentAccount = this->database.getStudentAccount(id);

    if (studentAccount) {
        logInfo("Student Account found");
        return studentAccount.get();
    } else {
        logError("Student Account not found in system");
        throw NotFoundException("Student Account identified by student " + std::to_string(id));
    }
}

bool StudentSystem::removeAccount(const StudentAccount &studentAccount) {
    logInfo("Request to remove Student Account " + studentAccount.getDescription() + " from system");
    return this->database.remove(studentAccount);
}

bool StudentSystem::updateAccount(const Student &student, const StudentAccount &updatedStudentAccount) {
    try {
        logInfo("Attempting to update Student Account for Lecturer " + student.getDescription());
        return this->database.update(student, updatedStudentAccount);
    } catch (KeyMismatch &m) {
        logError("Failed to update Student Account as Key Mismatch occurred " + string(m.what()));
        throw m;
    }
}

StudentSystem& StudentSystem::operator=(const StudentSystem &system) {
    this->database = DatabaseManager(system.database);

    return *this;
}