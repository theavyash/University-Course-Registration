#include "../include/FileManager.h"
#include "../include/RegistrationException.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Constructor
FileManager::FileManager(const string& adminsFilePath) 
    : adminsFilePath(adminsFilePath) {}

// Check if file exists
bool FileManager::fileExists() const {
    ifstream file(adminsFilePath);
    return file.good();
}

// Save all admins to file (overwrite mode)
void FileManager::saveAdmins(const vector<Admin>& admins) const {
    ofstream file(adminsFilePath, ios::trunc);
    
    if (!file.is_open()) {
        throw RegistrationException("Error: Cannot open admins file for writing: " + adminsFilePath);
    }
    
    for (const auto& admin : admins) {
        file << admin.serialize() << endl;
    }
    
    file.close();
    cout << "✓ Admin data saved successfully (" << admins.size() << " admins).\n";
}

// Load all admins from file
vector<Admin> FileManager::loadAdmins() const {
    vector<Admin> admins;
    ifstream file(adminsFilePath);
    
    if (!file.is_open()) {
        // File doesn't exist yet - return empty vector
        cout << "No admin file found. Starting with empty admin list.\n";
        return admins;
    }
    
    string line;
    int lineNum = 0;
    
    while (getline(file, line)) {
        lineNum++;
        
        if (line.empty()) {
            continue; // Skip empty lines
        }
        
        try {
            Admin admin = Admin::deserialize(line);
            admins.push_back(admin);
        } catch (const exception& e) {
            cerr << "Warning: Error loading admin on line " << lineNum 
                 << ": " << e.what() << endl;
            // Continue loading other admins
        }
    }
    
    file.close();
    cout << "✓ Loaded " << admins.size() << " admin(s) from file.\n";
    
    return admins;
}

// Append a single admin to the file
void FileManager::appendAdmin(const Admin& admin) const {
    ofstream file(adminsFilePath, ios::app);
    
    if (!file.is_open()) {
        throw RegistrationException("Error: Cannot open admins file for appending: " + adminsFilePath);
    }
    
    file << admin.serialize() << endl;
    file.close();
    
    cout << "✓ Admin added to file: " << admin.getUsername() << endl;
}