#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Course.h"
#include "Student.h"
#include <vector>
#include <string>

using namespace std;

// Admin class - demonstrates INHERITANCE and POLYMORPHISM
class Admin : public User {
public:
    // Constructors
    Admin();
    Admin(string username, string password, string email, string name, string userID);

    // POLYMORPHISM: Override pure virtual functions from User
    void displayMenu() override;
    string getUserType() const override;
    
    // Admin-specific course management functions
    void addCourse(vector<Course>& courses, const Course& newCourse);
    bool removeCourse(vector<Course>& courses, const string& courseCode);
    bool modifyCourse(vector<Course>& courses, const string& courseCode);
    void viewAllCourses(const vector<Course>& courses) const;
    
    // View enrolled students in a specific course
    void viewCourseEnrollments(const vector<Course>& courses, const string& courseCode) const;
    
    // View all students in the system
    void viewAllStudents(const vector<Student>& students) const;
    
    // Serialize for file I/O
    string serialize() const;
    
    // Deserialize from file
    static Admin deserialize(const string& data);
};

#endif // ADMIN_H