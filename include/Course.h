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
    
    // Schedule information
    std::string dayOfWeek;
    std::string startTime;
    std::string endTime;

    // Helper method
    int timeToMinutes(const std::string& time) const;

public:
    Course();
    Course(const std::string& code, const std::string& title, int capacity);
    Course(const std::string& code, const std::string& title, int capacity,
           const std::string& dayOfWeek, const std::string& startTime, const std::string& endTime);

    const std::string& getCode() const;
    const std::string& getTitle() const;
    int getCapacity() const;
    int seatsRemaining() const;
    const std::vector<std::string>& getEnrolledStudentIDs() const;
    
    const std::string& getDayOfWeek() const;
    const std::string& getStartTime() const;
    const std::string& getEndTime() const;
    
    void setSchedule(const std::string& dayOfWeek, const std::string& startTime, const std::string& endTime);
    
    // Member 3: Setters for Admin to modify courses
    void setCourseName(const std::string& name);
    void setCapacity(int newCapacity);
    std::string getCourseName() const;

    bool isStudentEnrolled(const std::string& studentID) const;
    void enrollStudent(const std::string& studentID);
    void dropStudent(const std::string& studentID);
    
    bool hasTimeConflict(const Course& other) const;
};

#endif