#include "../include/Course.h"
#include "../include/CustomExceptions.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

Course::Course() : code(""), title(""), capacity(0), dayOfWeek(""), startTime(""), endTime("") {}

Course::Course(const std::string& code, const std::string& title, int capacity)
    : code(code), title(title), capacity(capacity), dayOfWeek(""), startTime(""), endTime("") {}

Course::Course(const std::string& code, const std::string& title, int capacity,
               const std::string& dayOfWeek, const std::string& startTime, const std::string& endTime)
    : code(code), title(title), capacity(capacity), dayOfWeek(dayOfWeek), startTime(startTime), endTime(endTime) {}

const std::string& Course::getCode() const {
    return code;
}

const std::string& Course::getTitle() const {
    return title;
}

int Course::getCapacity() const {
    return capacity;
}

int Course::seatsRemaining() const {
    return capacity - static_cast<int>(enrolledStudentIDs.size());
}

const std::vector<std::string>& Course::getEnrolledStudentIDs() const {
    return enrolledStudentIDs;
}

bool Course::isStudentEnrolled(const std::string& studentID) const {
    return std::find(enrolledStudentIDs.begin(), enrolledStudentIDs.end(), studentID) != enrolledStudentIDs.end();
}

void Course::enrollStudent(const std::string& studentID) {
    if (isStudentEnrolled(studentID)) {
        throw DuplicateEntryException("student enrollment", studentID + " in " + code);
    }
    if (seatsRemaining() <= 0) {
        throw CourseFullException(code);
    }
    enrolledStudentIDs.push_back(studentID);
}

void Course::dropStudent(const std::string& studentID) {
    auto it = std::find(enrolledStudentIDs.begin(), enrolledStudentIDs.end(), studentID);
    if (it == enrolledStudentIDs.end()) {
        throw RegistrationException("Student not enrolled in " + code);
    }
    enrolledStudentIDs.erase(it);
}

// Schedule getters
const std::string& Course::getDayOfWeek() const {
    return dayOfWeek;
}

const std::string& Course::getStartTime() const {
    return startTime;
}

const std::string& Course::getEndTime() const {
    return endTime;
}

// Schedule setter
void Course::setSchedule(const std::string& dayOfWeek, const std::string& startTime, const std::string& endTime) {
    this->dayOfWeek = dayOfWeek;
    this->startTime = startTime;
    this->endTime = endTime;
}

// Helper method to convert time string (HH:MM) to minutes since midnight
int Course::timeToMinutes(const std::string& time) const {
    if (time.empty()) return 0;
    std::istringstream iss(time);
    int hours, minutes;
    char colon;
    if (iss >> hours >> colon >> minutes) {
        return hours * 60 + minutes;
    }
    return 0;
}

// Time conflict detection
bool Course::hasTimeConflict(const Course& other) const {
    // If either course has no schedule, no conflict
    if (dayOfWeek.empty() || other.dayOfWeek.empty()) {
        return false;
    }
    
    // Courses must be on the same day to conflict
    if (dayOfWeek != other.dayOfWeek) {
        return false;
    }
    
    // Convert times to minutes for easier comparison
    int thisStart = timeToMinutes(startTime);
    int thisEnd = timeToMinutes(endTime);
    int otherStart = timeToMinutes(other.startTime);
    int otherEnd = timeToMinutes(other.endTime);
    
    // Check for overlap: courses conflict if their time ranges overlap
    return (thisStart < otherEnd && thisEnd > otherStart);
}

// Member 3: Setters for Admin to modify courses
void Course::setCourseName(const std::string& name) {
    this->title = name;
}

void Course::setCapacity(int newCapacity) {
    this->capacity = newCapacity;
}

std::string Course::getCourseName() const {
    return title;
}