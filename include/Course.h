#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>

class Course {
private:
    std::string code;
    std::string title;
    int capacity;
    std::vector<std::string> enrolledStudentIDs;
    
    // Schedule information (Member 2: Schedule data management)
    std::string dayOfWeek;      // e.g., "Monday", "Tuesday"
    std::string startTime;      // e.g., "09:00" (24-hour format)
    std::string endTime;         // e.g., "10:30" (24-hour format)

    // Helper method to convert time string to minutes for comparison
    int timeToMinutes(const std::string& time) const;

public:
    Course();
    Course(const std::string& code, const std::string& title, int capacity);
    // Constructor with schedule (Member 2: Enhanced Course class)
    Course(const std::string& code, const std::string& title, int capacity,
           const std::string& dayOfWeek, const std::string& startTime, const std::string& endTime);

    const std::string& getCode() const;
    const std::string& getTitle() const;
    int getCapacity() const;
    int seatsRemaining() const;
    const std::vector<std::string>& getEnrolledStudentIDs() const;
    
    // Schedule getters (Member 2: Schedule data management)
    const std::string& getDayOfWeek() const;
    const std::string& getStartTime() const;
    const std::string& getEndTime() const;
    
    // Schedule setters
    void setSchedule(const std::string& dayOfWeek, const std::string& startTime, const std::string& endTime);
    
    // NEW: Setters for Admin to modify courses (Member 3)
    void setCourseName(const std::string& name);
    void setCapacity(int newCapacity);
    std::string getCourseName() const;

    bool isStudentEnrolled(const std::string& studentID) const;
    void enrollStudent(const std::string& studentID);
    void dropStudent(const std::string& studentID);
    
    // Time conflict detection (Member 2: Time conflict detection)
    bool hasTimeConflict(const Course& other) const;
};

#endif