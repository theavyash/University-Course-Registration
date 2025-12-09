    #include "../include/RegistrationSystem.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

RegistrationSystem::RegistrationSystem(const std::string& studentsFilePath, const std::string& coursesFilePath)
    : studentsFilePath(studentsFilePath), coursesFilePath(coursesFilePath) {}

void RegistrationSystem::loadData() {
    loadCourses();
    loadStudents();
}

void RegistrationSystem::saveData() const {
    saveCourses();
    saveStudents();
}

const std::vector<Course>& RegistrationSystem::getCourses() const {
    return courses;
}

const std::vector<Student>& RegistrationSystem::getStudents() const {
    return students;
}

Course* RegistrationSystem::findCourse(const std::string& code) {
    auto it = std::find_if(courses.begin(), courses.end(), [&](const Course& c) {
        return c.getCode() == code;
    });
    return it == courses.end() ? nullptr : &(*it);
}

Student* RegistrationSystem::findStudent(const std::string& username) {
    auto it = std::find_if(students.begin(), students.end(), [&](const Student& s) {
        return s.getUsername() == username;
    });
    return it == students.end() ? nullptr : &(*it);
}

Student& RegistrationSystem::createStudent(const std::string& username,
                                           const std::string& password,
                                           const std::string& email,
                                           const std::string& name,
                                           const std::string& userID,
                                           const std::string& studentID,
                                           const std::string& major,
                                           double gpa) {
    if (findStudent(username) != nullptr) {
        throw RegistrationException("Username already exists");
    }
    students.emplace_back(username, password, email, name, userID, studentID, major, gpa);
    return students.back();
}

Student* RegistrationSystem::login(const std::string& username, const std::string& password) {
    Student* student = findStudent(username);
    if (student == nullptr || student->getPassword() != password) {
        return nullptr;
    }
    return student;
}

void RegistrationSystem::registerForCourse(Student& student, const std::string& courseCode) {
    Course* course = findCourse(courseCode);
    if (!course) {
        throw RegistrationException("Course " + courseCode + " not found");
    }
    
    // Member 2: Time conflict detection
    // Check for time conflicts with already enrolled courses
    const auto& enrolledCourseCodes = student.getEnrolledCourses();
    for (const auto& enrolledCode : enrolledCourseCodes) {
        Course* enrolledCourse = findCourse(enrolledCode);
        if (enrolledCourse && course->hasTimeConflict(*enrolledCourse)) {
            throw RegistrationException("Time conflict: " + courseCode + " conflicts with " + enrolledCode + 
                                      " (Both on " + course->getDayOfWeek() + ")");
        }
    }
    
    // If no conflicts, proceed with enrollment
    course->enrollStudent(student.getStudentID());
    student.addCourse(courseCode);
}

void RegistrationSystem::dropCourse(Student& student, const std::string& courseCode) {
    Course* course = findCourse(courseCode);
    if (!course) {
        throw RegistrationException("Course " + courseCode + " not found");
    }
    course->dropStudent(student.getStudentID());
    student.removeCourse(courseCode);
}

void RegistrationSystem::listCourses() const {
    std::cout << "\n=====================================" << std::endl;
    std::cout << "      AVAILABLE COURSES              " << std::endl;
    std::cout << "=====================================" << std::endl;
    for (const auto& course : courses) {
        std::cout << "Code: " << course.getCode() << std::endl;
        std::cout << "Title: " << course.getTitle() << std::endl;
        std::cout << "Capacity: " << course.getCapacity() 
                  << " | Seats Remaining: " << course.seatsRemaining() << std::endl;
        // Member 2: Display schedule information
        if (!course.getDayOfWeek().empty()) {
            std::cout << "Schedule: " << course.getDayOfWeek() 
                      << " " << course.getStartTime() 
                      << " - " << course.getEndTime() << std::endl;
        }
        std::cout << "-------------------------------------" << std::endl;
    }
    std::cout << std::endl;
}

std::vector<std::string> RegistrationSystem::split(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        if (!item.empty()) {
            tokens.push_back(item);
        }
    }
    return tokens;
}

void RegistrationSystem::loadCourses() {
    std::ifstream file(coursesFilePath);
    if (!file.is_open()) {
        std::cout << "No course file found. Starting with a sample course list." << std::endl;
    } else {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string code, title, capacityStr, dayOfWeek, startTime, endTime, enrolledStr;
            
            // Member 2: Load schedule data from file
            std::getline(ss, code, '|');
            std::getline(ss, title, '|');
            std::getline(ss, capacityStr, '|');
            std::getline(ss, dayOfWeek, '|');
            std::getline(ss, startTime, '|');
            std::getline(ss, endTime, '|');
            std::getline(ss, enrolledStr, '|');

            int capacity = std::stoi(capacityStr);
            Course course(code, title, capacity, dayOfWeek, startTime, endTime);
            auto enrolled = split(enrolledStr, ',');
            for (const auto& id : enrolled) {
                try {
                    course.enrollStudent(id);
                } catch (const std::exception&) {
                    // ignore duplicates during load
                }
            }
            courses.push_back(course);
        }
    }

    if (courses.empty()) {
        // Member 2: Provide starter data with schedule information
        courses.emplace_back("CS101", "Intro to Programming", 30, "Monday", "09:00", "10:30");
        courses.emplace_back("MATH201", "Discrete Mathematics", 25, "Tuesday", "14:00", "15:30");
        courses.emplace_back("ENG150", "Academic Writing", 40, "Wednesday", "10:00", "11:30");
    }
}

void RegistrationSystem::loadStudents() {
    std::ifstream file(studentsFilePath);
    if (!file.is_open()) {
        std::cout << "No student file found. Starting with an empty student list." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string username, password, email, name, userID, studentID, major, gpaStr, coursesStr;
        std::getline(ss, username, '|');
        std::getline(ss, password, '|');
        std::getline(ss, email, '|');
        std::getline(ss, name, '|');
        std::getline(ss, userID, '|');
        std::getline(ss, studentID, '|');
        std::getline(ss, major, '|');
        std::getline(ss, gpaStr, '|');
        std::getline(ss, coursesStr, '|');

        double gpa = std::stod(gpaStr.empty() ? "0.0" : gpaStr);
        Student student(username, password, email, name, userID, studentID, major, gpa);
        student.setEnrolledCourses(split(coursesStr, ','));
        students.push_back(student);
    }
}

void RegistrationSystem::saveCourses() const {
    std::ofstream file(coursesFilePath, std::ios::trunc);
    for (const auto& course : courses) {
        // Member 2: Save schedule data to file
        file << course.getCode() << '|'
             << course.getTitle() << '|'
             << course.getCapacity() << '|'
             << course.getDayOfWeek() << '|'
             << course.getStartTime() << '|'
             << course.getEndTime() << '|';
        const auto& enrolled = course.getEnrolledStudentIDs();
        for (size_t i = 0; i < enrolled.size(); ++i) {
            file << enrolled[i];
            if (i + 1 < enrolled.size()) file << ',';
        }
        file << '\n';
    }
}

void RegistrationSystem::saveStudents() const {
    std::ofstream file(studentsFilePath, std::ios::trunc);
    for (const auto& student : students) {
        file << student.getUsername() << '|'
             << student.getPassword() << '|'
             << student.getEmail() << '|'
             << student.getName() << '|'
             << student.getUserID() << '|'
             << student.getStudentID() << '|'
             << student.getMajor() << '|'
             << student.getGPA() << '|';

        const auto& enrolled = student.getEnrolledCourses();
        for (size_t i = 0; i < enrolled.size(); ++i) {
            file << enrolled[i];
            if (i + 1 < enrolled.size()) file << ',';
        }
        file << '\n';
    }
}

