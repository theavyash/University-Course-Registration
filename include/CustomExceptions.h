#ifndef CUSTOM_EXCEPTIONS_H
#define CUSTOM_EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Base exception for registration system (already exists, but included for completeness)
class RegistrationException : public std::runtime_error {
public:
    explicit RegistrationException(const std::string& message)
        : std::runtime_error(message) {}
};

// Exception for when a course is at full capacity
class CourseFullException : public RegistrationException {
public:
    explicit CourseFullException(const std::string& courseCode)
        : RegistrationException("Course " + courseCode + " is full. No seats available.") {}
};

// Exception for time conflicts between courses
class TimeConflictException : public RegistrationException {
private:
    std::string course1;
    std::string course2;
    std::string conflictDay;
    
public:
    TimeConflictException(const std::string& c1, const std::string& c2, const std::string& day)
        : RegistrationException("Time conflict: " + c1 + " conflicts with " + c2 + " on " + day),
          course1(c1), course2(c2), conflictDay(day) {}
    
    std::string getCourse1() const { return course1; }
    std::string getCourse2() const { return course2; }
    std::string getConflictDay() const { return conflictDay; }
};

// Exception for invalid input (email, ID, etc.)
class InvalidInputException : public RegistrationException {
private:
    std::string fieldName;
    std::string invalidValue;
    
public:
    InvalidInputException(const std::string& field, const std::string& value, const std::string& reason)
        : RegistrationException("Invalid " + field + ": " + reason),
          fieldName(field), invalidValue(value) {}
    
    std::string getFieldName() const { return fieldName; }
    std::string getInvalidValue() const { return invalidValue; }
};

// Exception for file I/O errors
class FileException : public RegistrationException {
private:
    std::string fileName;
    std::string operation;
    
public:
    FileException(const std::string& file, const std::string& op)
        : RegistrationException("File error: Cannot " + op + " file '" + file + "'"),
          fileName(file), operation(op) {}
    
    std::string getFileName() const { return fileName; }
    std::string getOperation() const { return operation; }
};

// Exception for authentication failures
class AuthenticationException : public RegistrationException {
public:
    explicit AuthenticationException(const std::string& message)
        : RegistrationException("Authentication failed: " + message) {}
};

// Exception for duplicate entries
class DuplicateEntryException : public RegistrationException {
public:
    explicit DuplicateEntryException(const std::string& entryType, const std::string& value)
        : RegistrationException("Duplicate " + entryType + ": '" + value + "' already exists") {}
};

#endif // CUSTOM_EXCEPTIONS_H