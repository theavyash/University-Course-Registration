#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include <string>
#include <regex>

using namespace std;

// InputValidator class provides validation utilities for user input
class InputValidator {
public:
    // Email validation
    static bool isValidEmail(const string& email);
    
    // Student ID validation (format: S followed by 4-6 digits)
    static bool isValidStudentID(const string& studentID);
    
    // User ID validation (format: U followed by digits)
    static bool isValidUserID(const string& userID);
    
    // Course code validation (format: 2-4 letters followed by 3 digits)
    static bool isValidCourseCode(const string& courseCode);
    
    // GPA validation (range: 0.0 - 4.0)
    static bool isValidGPA(double gpa);
    
    // Time format validation (HH:MM in 24-hour format)
    static bool isValidTimeFormat(const string& time);
    
    // Day of week validation
    static bool isValidDayOfWeek(const string& day);
    
    // Password strength validation (minimum 6 characters)
    static bool isValidPassword(const string& password);
    
    // Username validation (alphanumeric, 3-20 characters)
    static bool isValidUsername(const string& username);
    
    // Check if string is not empty
    static bool isNotEmpty(const string& input);
    
    // Check if string is a valid integer
    static bool isValidInteger(const string& input);
    
    // Check if string is a valid positive integer
    static bool isValidPositiveInteger(const string& input);
    
    // Validate capacity (positive integer, reasonable range)
    static bool isValidCapacity(int capacity);
};

#endif // INPUT_VALIDATOR_H