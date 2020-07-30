#include "headers/Announcement.h"

using std::string;

int Announcement::lastID = 1;

Announcement::Announcement(const Module &module, const Lecturer &lecturer, std::string subject, std::string announcement) : module(module), lecturer(lecturer) {
    this->announcement = announcement;
    this->subject = subject;
    this->id = lastID++;
}

Announcement::Announcement(int id, const Module &module, const Lecturer &lecturer, std::string subject, std::string announcement) : module(module), lecturer(lecturer) {
    this->announcement = announcement;
    this->subject = subject;
    this->id = id;
}

int Announcement::getLastID() {
    return lastID;
}

void Announcement::setLastID(int lastID) {
    Announcement::lastID = lastID;
}

int Announcement::getID() const {
    return id;
}

void Announcement::setID(int id) {
    this->id = id;
}

Module Announcement::getModule() const {
    return module;
}

void Announcement::setModule(const Module &module) {
    this->module = module;
}

Lecturer Announcement::getLecturer() const {
    return lecturer;
}

void Announcement::setLecturer(const Lecturer &lecturer) {
    this->lecturer = lecturer;
}

string Announcement::getSubject() const {
    return subject;
}

void Announcement::setSubject(string subject) {
    this->subject = subject;
}

string Announcement::getAnnouncementText() const {
    return announcement;
}

void Announcement::setAnnouncementText(string announcement) {
    this->announcement = announcement;
}

string Announcement::getObjectType() const {
    return "Announcement";
}

Tables Announcement::getTable() const {
    return ANNOUNCEMENTS;
}

string Announcement::getDescription() const {
    return "Module: " + module.getDescription() + ", Lecturer: " + lecturer.getDescription() + ", Announcement: " + announcement;
}