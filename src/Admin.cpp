#include "../include/Admin.h"
#include "../include/CustomExceptions.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

using namespace std;

// Constructors
Admin::Admin() : User() {}

Admin::Admin(string username, string password, string email, string name, string userID) 
    : User(username, password, email, name, userID) {}

// POLYMORPHISM: Override getUserType
string Admin::getUserType() const {
    return "ADMIN";
}

// POLYMORPHISM: Override displayMenu - shows admin menu
void Admin::displayMenu() {
    cout << "\n========================================\n";
    cout << "          ADMIN MENU                   \n";
    cout << "========================================\n";
    cout << " 1. Add Course\n";
    cout << " 2. Remove Course\n";
    cout << " 3. Modify Course\n";
    cout << " 4. View All Courses\n";
    cout << " 5. View Course Enrollments\n";
    cout << " 6. View All Students\n";
    cout << " 7. View My Info\n";
    cout << " 8. Logout\n";
    cout << "========================================\n";
    cout << "Enter your choice: ";
}

// Add a new course to the system
void Admin::addCourse(vector<Course>& courses, const Course& newCourse) {
    // Check if course code already exists
    for (const auto& course : courses) {
        if (course.getCode() == newCourse.getCode()) {
            throw RegistrationException("Course code already exists: " + newCourse.getCode());
        }
    }
    
    courses.push_back(newCourse);
    cout << "\n✓ Course added successfully: " << newCourse.getCode() 
         << " - " << newCourse.getTitle() << endl;
}

// Remove a course from the system
bool Admin::removeCourse(vector<Course>& courses, const string& courseCode) {
    auto it = remove_if(courses.begin(), courses.end(),
        [&courseCode](const Course& c) {
            return c.getCode() == courseCode;
        });
    
    if (it != courses.end()) {
        courses.erase(it, courses.end());
        cout << "\n✓ Course removed successfully: " << courseCode << endl;
        return true;
    }
    
    throw RegistrationException("Course not found: " + courseCode);
}

// Modify an existing course
bool Admin::modifyCourse(vector<Course>& courses, const string& courseCode) {
    // Find the course
    Course* courseToModify = nullptr;
    for (auto& course : courses) {
        if (course.getCode() == courseCode) {
            courseToModify = &course;
            break;
        }
    }
    
    if (!courseToModify) {
        throw RegistrationException("Course not found: " + courseCode);
    }
    
    cout << "\n=== Modify Course: " << courseCode << " ===\n";
    cout << "Current course name: " << courseToModify->getTitle() << endl;
    
    int choice;
    cout << "\nWhat would you like to modify?\n";
    cout << "1. Course Title\n";
    cout << "2. Capacity\n";
    cout << "3. Schedule (Day and Time)\n";
    cout << "0. Cancel\n";
    cout << "Choice: ";
    cin >> choice;
    cin.ignore();
    
    switch (choice) {
        case 1: {
            string newTitle;
            cout << "Enter new course title: ";
            getline(cin, newTitle);
            courseToModify->setCourseName(newTitle);
            cout << "✓ Course title updated.\n";
            break;
        }
        case 2: {
            int newCapacity;
            cout << "Enter new capacity: ";
            cin >> newCapacity;
            courseToModify->setCapacity(newCapacity);
            cout << "✓ Capacity updated.\n";
            break;
        }
        case 3: {
            string newDay, newStartTime, newEndTime;
            cout << "Enter new day (e.g., Monday): ";
            getline(cin, newDay);
            cout << "Enter new start time (HH:MM): ";
            getline(cin, newStartTime);
            cout << "Enter new end time (HH:MM): ";
            getline(cin, newEndTime);
            
            courseToModify->setSchedule(newDay, newStartTime, newEndTime);
            cout << "✓ Schedule updated.\n";
            break;
        }
        case 0:
            cout << "Modification cancelled.\n";
            return false;
        default:
            cout << "Invalid choice.\n";
            return false;
    }
    
    return true;
}

// View all courses in the system
void Admin::viewAllCourses(const vector<Course>& courses) const {
    if (courses.empty()) {
        cout << "\nNo courses available in the system.\n";
        return;
    }
    
    cout << "\n========================================\n";
    cout << "          ALL COURSES IN SYSTEM        \n";
    cout << "========================================\n";
    
    for (const auto& course : courses) {
        cout << "Code: " << course.getCode() << endl;
        cout << "Title: " << course.getTitle() << endl;
        cout << "Capacity: " << course.getCapacity() 
             << " | Enrolled: " << course.getEnrolledStudentIDs().size()
             << " | Remaining: " << course.seatsRemaining() << endl;
        if (!course.getDayOfWeek().empty()) {
            cout << "Schedule: " << course.getDayOfWeek() 
                 << " " << course.getStartTime() 
                 << " - " << course.getEndTime() << endl;
        }
        cout << "----------------------------------------\n";
    }
    
    cout << "Total Courses: " << courses.size() << endl;
}

// View students enrolled in a specific course
void Admin::viewCourseEnrollments(const vector<Course>& courses, const string& courseCode) const {
    // Find the course
    const Course* targetCourse = nullptr;
    for (const auto& course : courses) {
        if (course.getCode() == courseCode) {
            targetCourse = &course;
            break;
        }
    }
    
    if (!targetCourse) {
        throw RegistrationException("Course not found: " + courseCode);
    }
    
    cout << "\n=== Enrollments for Course: " << courseCode << " ===\n";
    cout << "Course Title: " << targetCourse->getTitle() << endl;
    cout << "Enrolled Students: " << targetCourse->getEnrolledStudentIDs().size() 
         << " / " << targetCourse->getCapacity() << endl;
    cout << "----------------------------------------\n";
    
    const auto& enrolledIDs = targetCourse->getEnrolledStudentIDs();
    if (enrolledIDs.empty()) {
        cout << "No students enrolled yet.\n";
    } else {
        for (size_t i = 0; i < enrolledIDs.size(); i++) {
            cout << (i + 1) << ". Student ID: " << enrolledIDs[i] << endl;
        }
    }
}

// View all students in the system
void Admin::viewAllStudents(const vector<Student>& students) const {
    if (students.empty()) {
        cout << "\nNo students in the system.\n";
        return;
    }
    
    cout << "\n========================================\n";
    cout << "          ALL STUDENTS IN SYSTEM       \n";
    cout << "========================================\n";
    
    for (const auto& student : students) {
        cout << "Name: " << student.getName() << endl;
        cout << "Username: " << student.getUsername() << endl;
        cout << "Student ID: " << student.getStudentID() << endl;
        cout << "Major: " << student.getMajor() << endl;
        cout << "GPA: " << fixed << setprecision(2) << student.getGPA() << endl;
        cout << "Enrolled Courses: " << student.getTotalEnrolledCourses() << endl;
        cout << "----------------------------------------\n";
    }
    
    cout << "Total Students: " << students.size() << endl;
}

// Serialize admin data for file I/O
string Admin::serialize() const {
    // Format: username|password|email|name|userID|ADMIN
    return username + "|" + password + "|" + email + "|" + name + "|" + userID + "|ADMIN";
}

// Deserialize admin from file data
Admin Admin::deserialize(const string& data) {
    stringstream ss(data);
    string user, pass, mail, nm, uid, type;
    
    getline(ss, user, '|');
    getline(ss, pass, '|');
    getline(ss, mail, '|');
    getline(ss, nm, '|');
    getline(ss, uid, '|');
    getline(ss, type, '|');
    
    return Admin(user, pass, mail, nm, uid);
}