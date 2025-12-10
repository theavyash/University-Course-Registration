#include <iostream>
#include <vector>
#include "../include/User.h"
#include "../include/Student.h"
#include "../include/Admin.h"
#include "../include/RegistrationSystem.h"
#include "../include/FileManager.h"
#include "../include/RegistrationException.h"

using namespace std;

// Helper function to prompt for input
static string prompt(const string& label) {
    cout << label;
    string value;
    getline(cin, value);
    return value;
}

// Student session with polymorphism demonstration
static void studentSession(Student& student, RegistrationSystem& regSys) {
    bool inSession = true;
    User* userPtr = &student;  // POLYMORPHISM: Base class pointer to derived object
    
    while (inSession) {
        userPtr->displayMenu(); // RUNTIME POLYMORPHISM: Calls Student::displayMenu()
        string choice;
        getline(cin, choice);

        try {
            if (choice == "1") {
                student.displayStudentInfo();
            } else if (choice == "2") {
                student.displayEnrolledCourses();
            } else if (choice == "3") {
                inSession = false;
                cout << "Logging out...\n";
            } else {
                cout << "Invalid choice. Try again.\n";
            }
        } catch (const RegistrationException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

// Admin session with polymorphism demonstration
static void adminSession(Admin& admin, RegistrationSystem& regSys) {
    bool inSession = true;
    User* userPtr = &admin;  // POLYMORPHISM: Base class pointer to derived object
    
    while (inSession) {
        userPtr->displayMenu(); // RUNTIME POLYMORPHISM: Calls Admin::displayMenu()
        string choice;
        getline(cin, choice);

        try {
            if (choice == "1") {
                // Add Course
                cout << "\n--- Add New Course ---\n";
                string code = prompt("Course Code: ");
                string title = prompt("Course Title: ");
                int capacity;
                cout << "Capacity: ";
                cin >> capacity;
                cin.ignore();
                
                string day = prompt("Day of Week (e.g., Monday): ");
                string startTime = prompt("Start Time (HH:MM): ");
                string endTime = prompt("End Time (HH:MM): ");
                
                Course newCourse(code, title, capacity, day, startTime, endTime);
                
                // Get mutable reference to courses
                vector<Course> courses = regSys.getCourses();
                admin.addCourse(courses, newCourse);
                
                // Note: In a real system, you'd need to update RegistrationSystem
                // For now, this demonstrates the Admin functionality
                
            } else if (choice == "2") {
                // Remove Course
                cout << "\n--- Remove Course ---\n";
                string code = prompt("Enter course code to remove: ");
                
                vector<Course> courses = regSys.getCourses();
                admin.removeCourse(courses, code);
                
            } else if (choice == "3") {
                // Modify Course
                cout << "\n--- Modify Course ---\n";
                string code = prompt("Enter course code to modify: ");
                
                vector<Course> courses = regSys.getCourses();
                admin.modifyCourse(courses, code);
                
            } else if (choice == "4") {
                // View All Courses
                admin.viewAllCourses(regSys.getCourses());
                
            } else if (choice == "5") {
                // View Course Enrollments
                string code = prompt("Enter course code: ");
                admin.viewCourseEnrollments(regSys.getCourses(), code);
                
            } else if (choice == "6") {
                // View All Students
                admin.viewAllStudents(regSys.getStudents());
                
            } else if (choice == "7") {
                // View My Info
                admin.displayInfo();
                
            } else if (choice == "8") {
                inSession = false;
                cout << "Logging out...\n";
            } else {
                cout << "Invalid choice. Try again.\n";
            }
        } catch (const RegistrationException& e) {
            cout << "Error: " << e.what() << endl;
        } catch (const exception& e) {
            cout << "Unexpected error: " << e.what() << endl;
        }
    }
}

int main() {
    // Initialize systems with file paths
    RegistrationSystem regSys("data/students.txt", "data/courses.txt");
    FileManager fileManager("data/admins.txt");
    
    // Load data from files
    cout << "=== Loading System Data ===\n";
    regSys.loadData();
    vector<Admin> admins = fileManager.loadAdmins();
    
    // Create default admin if none exist
    if (admins.empty()) {
        cout << "\nNo admin accounts found. Creating default admin...\n";
        Admin defaultAdmin("admin", "admin123", "admin@university.edu", "System Admin", "A001");
        admins.push_back(defaultAdmin);
        fileManager.saveAdmins(admins);
        cout << "Default admin created (username: admin, password: admin123)\n";
    }
    
    bool running = true;

    while (running) {
        cout << "\n========================================\n";
        cout << "  UNIVERSITY COURSE REGISTRATION       \n";
        cout << "========================================\n";
        cout << "1. Student Sign Up\n";
        cout << "2. Student Login\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n";
        cout << "========================================\n";
        cout << "Choose an option: ";

        string choice;
        getline(cin, choice);

        try {
            if (choice == "1") {
                // Student Sign Up
                cout << "\n--- Student Sign Up ---\n";
                string username = prompt("Username: ");
                string password = prompt("Password: ");
                string email = prompt("Email: ");
                string name = prompt("Full Name: ");
                string studentID = prompt("Student ID: ");
                string major = prompt("Major: ");
                double gpa = 0.0;
                try {
                    gpa = stod(prompt("GPA (0.0-4.0): "));
                } catch (...) {
                    gpa = 0.0;
                }
                
                string userID = "U" + to_string(regSys.getStudents().size() + 1);
                Student& s = regSys.createStudent(username, password, email, name, 
                                                  userID, studentID, major, gpa);
                regSys.saveData();
                
                cout << "\n✓ Student account created for " << s.getName() << "!\n";
                cout << "You can now log in.\n";
                
            } else if (choice == "2") {
                // Student Login
                cout << "\n--- Student Login ---\n";
                string username = prompt("Username: ");
                string password = prompt("Password: ");
                
                Student* s = regSys.login(username, password);
                if (!s) {
                    cout << "✗ Invalid username or password.\n";
                } else {
                    cout << "\n✓ Welcome, " << s->getName() << "!\n";
                    cout << "User Type: " << s->getUserType() << endl; // Polymorphism
                    studentSession(*s, regSys);
                    regSys.saveData(); // Save after session ends
                }
                
            } else if (choice == "3") {
                // Admin Login
                cout << "\n--- Admin Login ---\n";
                string username = prompt("Username: ");
                string password = prompt("Password: ");
                
                // Find admin
                Admin* loggedInAdmin = nullptr;
                for (auto& admin : admins) {
                    if (admin.getUsername() == username && admin.getPassword() == password) {
                        loggedInAdmin = &admin;
                        break;
                    }
                }
                
                if (!loggedInAdmin) {
                    cout << "✗ Invalid admin credentials.\n";
                } else {
                    cout << "\n✓ Welcome, Admin " << loggedInAdmin->getName() << "!\n";
                    cout << "User Type: " << loggedInAdmin->getUserType() << endl; // Polymorphism
                    adminSession(*loggedInAdmin, regSys);
                    regSys.saveData(); // Save changes made by admin
                    fileManager.saveAdmins(admins); // Save admin data
                }
                
            } else if (choice == "4") {
                // Exit
                cout << "\n=== Saving System Data ===\n";
                regSys.saveData();
                fileManager.saveAdmins(admins);
                cout << "All data saved. Goodbye!\n";
                running = false;
            } else {
                cout << "Invalid option. Try again.\n";
            }
            
        } catch (const RegistrationException& e) {
            cout << "Registration Error: " << e.what() << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}