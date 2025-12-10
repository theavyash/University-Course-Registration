#include "../include/Student.h"
#include <iostream>
#include <algorithm>
using namespace std;

// Default Constructor
Student::Student() : User() {
    studentID = "";
    major = "";
    gpa = 0.0;
}

// Parameterized Constructor
Student::Student(string username, string password, string email, string name, 
                 string userID, string studentID, string major, double gpa)
    : User(username, password, email, name, userID) {
    this->studentID = studentID;
    this->major = major;
    this->gpa = gpa;
}

// Destructor
Student::~Student() {
    // Cleanup if needed
}

// Getters
string Student::getStudentID() const {
    return studentID;
}

string Student::getMajor() const {
    return major;
}

double Student::getGPA() const {
    return gpa;
}

vector<string> Student::getEnrolledCourses() const {
    return enrolledCourses;
}

// Setters
void Student::setStudentID(string studentID) {
    this->studentID = studentID;
}

void Student::setMajor(string major) {
    this->major = major;
}

void Student::setGPA(double gpa) {
    this->gpa = gpa;
}

void Student::setEnrolledCourses(const vector<string>& courses) {
    enrolledCourses = courses;
}

// Add a course to student's enrolled list
void Student::addCourse(string courseCode) {
    if (!isEnrolledIn(courseCode)) {
        enrolledCourses.push_back(courseCode);
        cout << "Course " << courseCode << " added (placeholder, Member 2 will handle capacity/conflicts)." << endl;
    } else {
        cout << "Already enrolled in " << courseCode << endl;
    }
}

// Remove a course from student's enrolled list
void Student::removeCourse(string courseCode) {
    auto it = find(enrolledCourses.begin(), enrolledCourses.end(), courseCode);
    if (it != enrolledCourses.end()) {
        enrolledCourses.erase(it);
        cout << "Course " << courseCode << " removed (placeholder)." << endl;
    } else {
        cout << "You are not enrolled in " << courseCode << endl;
    }
}

// Check if student is enrolled in a specific course
bool Student::isEnrolledIn(string courseCode) const {
    return find(enrolledCourses.begin(), enrolledCourses.end(), courseCode) 
           != enrolledCourses.end();
}

// Get total number of enrolled courses
int Student::getTotalEnrolledCourses() const {
    return enrolledCourses.size();
}

// POLYMORPHISM: Override displayMenu() from User
// POLYMORPHISM: Override displayMenu() from User
void Student::displayMenu() {
    cout << "\n========================================\n";
    cout << "         STUDENT MENU                   \n";
    cout << "========================================\n";
    cout << " 1. View My Information\n";
    cout << " 2. View My Enrolled Courses\n";
    cout << " 3. View All Available Courses\n";
    cout << " 4. Register for a Course\n";
    cout << " 5. Drop a Course\n";
    cout << " 6. Logout\n";
    cout << "========================================\n";
    cout << "Enter your choice: ";
}

// POLYMORPHISM: Override getUserType() from User
string Student::getUserType() const {
    return "STUDENT";
}

// Display student-specific information
void Student::displayStudentInfo() const {
    cout << "\n=====================================" << endl;
    cout << "      STUDENT INFORMATION            " << endl;
    cout << "=====================================" << endl;
    cout << "Name: " << name << endl;
    cout << "Username: " << username << endl;
    cout << "Email: " << email << endl;
    cout << "Student ID: " << studentID << endl;
    cout << "Major: " << major << endl;
    cout << "GPA: " << gpa << endl;
    cout << "Enrolled Courses: " << enrolledCourses.size() << endl;
    cout << "=====================================\n" << endl;
}

// Display all enrolled courses
void Student::displayEnrolledCourses() const {
    cout << "\n=====================================" << endl;
    cout << "      ENROLLED COURSES               " << endl;
    cout << "=====================================" << endl;
    
    if (enrolledCourses.empty()) {
        cout << "You are not enrolled in any courses yet." << endl;
    } else {
        cout << "Total Courses: " << enrolledCourses.size() << endl;
        cout << "-------------------------------------" << endl;
        for (int i = 0; i < enrolledCourses.size(); i++) {
            cout << (i + 1) << ". " << enrolledCourses[i] << endl;
        }
    }
    cout << "=====================================\n" << endl;
}