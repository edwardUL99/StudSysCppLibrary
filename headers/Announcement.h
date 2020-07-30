#ifndef ANNOUNCEMENT_H
#define ANNOUNCEMENT_H

#include "DatabaseItem.h"
#include "Module.h"
#include "Lecturer.h"

//will keep time separate and have a struct as part of UI as e.g struct ui__announcement
class Announcement : public DatabaseItem
{
    private:
        int id;
        static int lastID;
        Module module;
        Lecturer lecturer;
        std::string subject;
        std::string announcement;

    public:
        Announcement(const Module &module, const Lecturer &lecturer, std::string subject, std::string announcement);
        Announcement(int id, const Module &module, const Lecturer &lecturer, std::string subject, std::string announcement);
        virtual ~Announcement() = default;
        static int getLastID();
        static void setLastID(int lastID);
        int getID() const;
        void setID(int id);
        Module getModule() const;
        void setModule(const Module &module);
        Lecturer getLecturer() const;
        void setLecturer(const Lecturer &lecturer);
        std::string getSubject() const;
        void setSubject(std::string subject);
        std::string getAnnouncementText() const;
        void setAnnouncementText(std::string announcement);
        virtual std::string getObjectType() const override;
        virtual Tables getTable() const override;
        virtual std::string getDescription() const override;
};

#endif