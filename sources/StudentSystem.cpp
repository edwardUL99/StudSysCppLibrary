#include "headers/StudentSystem.h"
#include "headers/DuplicateException.h"
#include "headers/NotFoundException.h"
#include "headers/KeyMismatch.h"
#include "headers/StudentRegistration.h"

using std::string;

StudentSystem::StudentSystem() {}

StudentSystem::StudentSystem(const StudentSystem &system) {
    this->database = DatabaseManager(system.database);
}

void StudentSystem::startSystem(string database, string user, string pass, string host) {
    this->database.connectToDatabase(database, user, pass, host);
}

string StudentSystem::recordLogin(const Account &account) {
    string date = "FIRST_LOGIN";

    ResultSet *res = NULL;

    try {
        const StudentAccount &student = dynamic_cast<const StudentAccount&>(account);
        string id = std::to_string(student.getStudent().getID());

        res = this->database.executeQuery("SELECT loginTime FROM student_logins WHERE loginTime >= ALL(SELECT loginTime FROM student_logins) AND id = " + id + ";");

        if (res->next()) {
            date = res->getString("loginTime");
        }

        this->database.execute("INSERT INTO student_logins (id) VALUES (" + id + ");");
    } catch (std::bad_cast &b) {}

    try {
        const LecturerAccount &lecturer = dynamic_cast<const LecturerAccount&>(account);
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
    if (this->database.contains(lecturer)) {
        throw DuplicateException(lecturer.getDescription());
    } else {
        return this->database.add(lecturer);
    }
}

Lecturer StudentSystem::getLecturer(string email) {
    boost::optional<Lecturer> lecturer = this->database.getLecturer(email);

    if (lecturer) {
        return lecturer.get();
    } else {
        throw NotFoundException("Lecturer identified by " + email);
    }
}

bool StudentSystem::removeLecturer(const Lecturer &lecturer) {
    return this->database.remove(lecturer);
}

bool StudentSystem::updateLecturer(string email, const Lecturer &updatedLecturer) {
    try {
        return this->database.update(email, updatedLecturer);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

bool StudentSystem::addStudent(const Student &student) {
    if (this->database.contains(student)) {
        throw DuplicateException(student.getDescription());
    } else {
        return this->database.add(student);
    }
}

Student StudentSystem::getStudent(int id) {
    boost::optional<Student> student = this->database.getStudent(id);

    if (student) {
        return student.get();
    } else {
        throw NotFoundException("Student identified by " + std::to_string(id));
    }
}

bool StudentSystem::removeStudent(const Student &student) {
    return this->database.remove(student);
}

bool StudentSystem::updateStudent(int id, const Student &updatedStudent) {
    try {
        return this->database.update(id, updatedStudent);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

int StudentSystem::getStudentID(string email) {
    for (const Student &s : this->database.getAllStudents()) {
        if (s.getEmail() == email) {
            return s.getID();
        }
    }

    return -1;
}

bool StudentSystem::addCourse(const Course &course) {
    if (this->database.contains(course)) {
        throw DuplicateException(course.getDescription());
    } else {
        return this->database.add(course);
    }
}

Course StudentSystem::getCourse(string id) {
    boost::optional<Course> course = this->database.getCourse(id);

    if (course) {
        return course.get();
    } else {
        throw NotFoundException("Course identified by " + id);
    }
}

bool StudentSystem::removeCourse(const Course &course) {
    return this->database.remove(course);
}

bool StudentSystem::updateCourse(string id, const Course &updatedCourse) {
    try {
        return this->database.update(id, updatedCourse);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

bool StudentSystem::addModule(const Module &module) {
    if (this->database.contains(module)) {
        throw DuplicateException(module.getDescription());
    } else {
        return this->database.add(module);
    }
}

Module StudentSystem::getModule(string code) {
    boost::optional<Module> module = this->database.getModule(code);

    if (module) {
        return module.get();
    } else {
        throw NotFoundException("Module identified by " + code);
    }
}

bool StudentSystem::removeModule(const Module &module) {
    return this->database.remove(module);
}

bool StudentSystem::updateModule(string code, const Module &updatedModule) {
    try {
        return this->database.update(code, updatedModule);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

std::vector<Module> StudentSystem::getModules() {
    return this->database.getAllModules();
}

bool StudentSystem::addAnnouncement(const Announcement &announcement) {
    if (this->database.contains(announcement)) {
        throw DuplicateException(announcement.getDescription());
    } else {
        return this->database.add(announcement);
    }
}

bool StudentSystem::removeAnnouncement(const Announcement &announcement) {
    return this->database.remove(announcement);
}

std::vector<Announcement> StudentSystem::getModuleAnnouncements(const Module &module) {
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
        return this->database.update(announcementNumber, moduleCode, updatedAnnouncement);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

string StudentSystem::getAnnouncementTime(const Announcement &announcement) {
    ResultSet *res = this->database.executeQuery("SELECT time_created FROM announcements WHERE id = " + std::to_string(announcement.getID()) + " AND module = '" + announcement.getModule().getCode() + "';");

    string time = "";
    if (res->next()) {
        time = res->getString("time_created");
    }

    delete res;

    return time;
}

bool StudentSystem::registerStudentModule(const Student &student, const Module &module) {
    StudentRegistration registration(student, module);

    if (this->database.contains(registration)) {
        throw DuplicateException(registration.getDescription());
    } else {
        return this->database.add(registration);
    }
}

bool StudentSystem::unregisterStudentModule(const Student &student, const Module &module) {
    StudentRegistration registration(student, module);

    return this->database.remove(registration);
}

std::vector<Module> StudentSystem::getStudentRegisteredModules(const Student &student) {
    std::vector<Module> modules;

    for (const StudentRegistration &registration : this->database.getAllStudentRegistrations()) {
        if (registration.getStudent().getID() == student.getID()) {
            modules.push_back(registration.getModule());
        }
    }

    return modules;
}

std::vector<Student> StudentSystem::getStudentsRegisteredOnModule(const Module &module) {
    std::vector<Student> students;

    for (const StudentRegistration &reg : this->database.getAllStudentRegistrations()) {
        if (reg.getModule().getCode() == module.getCode()) {
            students.push_back(reg.getStudent());
        }
    }

    //string query = "SELECT student FROM student_registrations JOIN modules ON module = code WHERE module = '" + module.getCode() + "';"; maybe use this and result set instead and then getStudent (from id)

    return students;
}

bool StudentSystem::addExam(const Exam &exam) {
    if (this->database.contains(exam)) {
        throw DuplicateException(exam.getDescription());
    } else {
        return this->database.add(exam);
    }
}

Exam StudentSystem::getExam(int id) {
    boost::optional<Exam> exam = this->database.getExam(id);

    if (exam) {
        return exam.get();
    } else {
        throw NotFoundException("Exam identified by " + std::to_string(id));
    }
}

bool StudentSystem::removeExam(const Exam &exam) {
    return this->database.remove(exam);
}

bool StudentSystem::updateExam(const Exam &oldExam, const Exam &updatedExam) {
    try {
        return this->database.update(oldExam, updatedExam);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

std::vector<Exam> StudentSystem::retrieveExamsByModule(const Module &module) {
    std::vector<Exam> exams;

    for (const Exam &exam : this->database.getAllExams()) {
        if (exam.getModule().getCode() == module.getCode()) {
            exams.push_back(exam);
        }
    }

    return exams;
    
    /* Or you could do as follows:
    string query = "SELECT id FROM exams LEFT JOIN modules ON module = code WHERE module = '" + module.getCode() + "';";
    ResultSet *res = this->database.executeQuery(query);

    while (res->next()) {
        exams.push_back(this->getExam(res->getInt("id")));
    }

    delete res;

    return exams;
    */
}

bool StudentSystem::examTaken(const Exam &exam) {
    ResultSet *res = this->database.executeQuery("SELECT * FROM exam_grades WHERE exam = " + std::to_string(exam.getID()) + ";");
    bool taken = res->next(); //if res has an item from next, the exam has been taken

    delete res;
    return taken;
}

bool StudentSystem::addExamGrade(const ExamGrade &examGrade) {
    if (this->database.contains(examGrade)) {
        throw DuplicateException(examGrade.getDescription());
    } else {
        return this->database.add(examGrade);
    }
}

ExamGrade StudentSystem::getExamGrade(const Student &student, const Exam &exam) {
    boost::optional<ExamGrade> examGrade = this->database.getExamGrade(student, exam);

    if (examGrade) {
        return examGrade.get();
    } else {
        throw NotFoundException("Exam Grade identified by Student " + std::to_string(student.getID()) + " and Exam " + std::to_string(exam.getID()));
    }
}

bool StudentSystem::removeExamGrade(const ExamGrade &examGrade) {
    return this->database.remove(examGrade);
}

bool StudentSystem::updateExamGrade(const Student &student, const Exam &exam, const ExamGrade &updatedExamGrade) {
    try {
        return this->database.update(student, exam, updatedExamGrade);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

ModuleGrade StudentSystem::getModuleGrade(const Module &module, const Student &student) {
    boost::optional<ModuleGrade> moduleGrade = this->database.getModuleGrade(module, student);

    if (moduleGrade) {
        return moduleGrade.get();
    } else {
        throw NotFoundException("Module Grade identified by module " + module.getCode() + " and student " + std::to_string(student.getID()));
    }
}

bool StudentSystem::addAccount(LecturerAccount lecturerAccount) {
    if (this->database.contains(lecturerAccount)) {
        throw DuplicateException(lecturerAccount.getDescription());
    } else {
        return this->database.add(lecturerAccount);
    }
}

LecturerAccount StudentSystem::getLecturerAccount(string email) {
    boost::optional<LecturerAccount> lecturerAccount = this->database.getLecturerAccount(email);

    if (lecturerAccount) {
        return lecturerAccount.get();
    } else {
        throw NotFoundException("Lecturer Account identified by lecturer " + email);
    }
}

bool StudentSystem::removeAccount(const LecturerAccount &lecturerAccount) {
    return this->database.remove(lecturerAccount);
}

bool StudentSystem::updateAccount(const Lecturer &lecturer, const LecturerAccount &updatedLecturerAccount) {
    try {
        return this->database.update(lecturer, updatedLecturerAccount);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

bool StudentSystem::addAccount(const StudentAccount &studentAccount) {
    if (this->database.contains(studentAccount)) {
        throw DuplicateException(studentAccount.getDescription());
    } else {
        return this->database.add(studentAccount);
    }
}

StudentAccount StudentSystem::getStudentAccount(int id) {
    boost::optional<StudentAccount> studentAccount = this->database.getStudentAccount(id);

    if (studentAccount) {
        return studentAccount.get();
    } else {
        throw NotFoundException("Student Account identified by student " + std::to_string(id));
    }
}

bool StudentSystem::removeAccount(const StudentAccount &studentAccount) {
    return this->database.remove(studentAccount);
}

bool StudentSystem::updateAccount(const Student &student, const StudentAccount &updatedStudentAccount) {
    try {
        return this->database.update(student, updatedStudentAccount);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

StudentSystem& StudentSystem::operator=(const StudentSystem &system) {
    this->database = DatabaseManager(system.database);

    return *this;
}