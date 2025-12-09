#ifndef REGISTRATION_SYSTEM_H
#define REGISTRATION_SYSTEM_H

#include "Course.h"
#include "Student.h"
#include "RegistrationException.h"
#include <string>
#include <vector>

class RegistrationSystem {
private:
    std::vector<Course> courses;
    std::vector<Student> students;
    std::string studentsFilePath;
    std::string coursesFilePath;

    Course* findCourse(const std::string& code);
    Student* findStudent(const std::string& username);

    static std::vector<std::string> split(const std::string& input, char delimiter);
    void loadCourses();
    void loadStudents();
    void saveCourses() const;
    void saveStudents() const;

public:
    RegistrationSystem(const std::string& studentsFilePath, const std::string& coursesFilePath);

    void loadData();
    void saveData() const;

    const std::vector<Course>& getCourses() const;
    const std::vector<Student>& getStudents() const;

    Student& createStudent(const std::string& username,
                           const std::string& password,
                           const std::string& email,
                           const std::string& name,
                           const std::string& userID,
                           const std::string& studentID,
                           const std::string& major,
                           double gpa);

    Student* login(const std::string& username, const std::string& password);

    void registerForCourse(Student& student, const std::string& courseCode);
    void dropCourse(Student& student, const std::string& courseCode);

    void listCourses() const;
};

#endif

