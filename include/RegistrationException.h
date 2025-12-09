#ifndef REGISTRATION_EXCEPTION_H
#define REGISTRATION_EXCEPTION_H

#include <stdexcept>
#include <string>

// Custom exception type for registration-related errors
class RegistrationException : public std::runtime_error {
public:
    explicit RegistrationException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif

