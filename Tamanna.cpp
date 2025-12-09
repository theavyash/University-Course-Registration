#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


// Base Class: User

class User {
protected:
    string username;
    string role;

public:
    User() {}
    User(string u, string r) : username(u), role(r) {}

    virtual void displayInfo() {
        cout << "Username: " << username << endl;
        cout << "Role: " << role << endl;
    }

    string getUsername() const { return username; }
    string getRole() const { return role; }

    virtual string serialize() const {
        return username + "," + role;
    }
};

// Derived Class: Admin

class Admin : public User {
public:
    Admin() {}
    Admin(string u) : User(u, "Admin") {}

    void displayInfo() override {
        cout << "=== Admin Information ===" << endl;
        User::displayInfo();
    }

    // Admin creates a new user (role = Student)
    User createUser(string username) {
        return User(username, "Student");
    }

    // Admin deletes a user
    bool deleteUser(vector<User>& users, string name) {
        for (size_t i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == name) {
                users.erase(users.begin() + i);
                return true;
            }
        }
        return false;
    }
};


// FileManager Class
// Handles all file I/O

class FileManager {
private:
    string filename;

public:
    FileManager(string file) : filename(file) {}

    // Save all users to file (overwrite)
    void saveAll(const vector<User>& users) {
        ofstream file(filename);
        if (!file) {
            cout << "Error saving users.\n";
            return;
        }

        for (const auto& u : users) {
            file << u.serialize() << endl;
        }
    }

    // Load all users from file
    vector<User> loadAll() {
        vector<User> users;
        ifstream file(filename);

        if (!file) {
            return users; // file didn't exist yet
        }

        string line;
        while (getline(file, line)) {
            size_t pos = line.find(',');
            if (pos == string::npos) continue;

            string name = line.substr(0, pos);
            string role = line.substr(pos + 1);

            users.push_back(User(name, role));
        }
        return users;
    }

    // Append one user at end of file
    void appendUser(const User& u) {
        ofstream file(filename, ios::app);
        if (!file) {
            cout << "Error appending user.\n";
            return;
        }

        file << u.serialize() << endl;
    }
};


// Main Program Demonstration

int main() {

    FileManager fm("users.txt");

    // Load existing users from file
    vector<User> users = fm.loadAll();

    // Create admin
    Admin admin("Tamanna");

    admin.displayInfo();

    // Create new user
    User s1 = admin.createUser("student1");
    users.push_back(s1);
    fm.appendUser(s1);

    // Create another user
    User s2 = admin.createUser("student2");
    users.push_back(s2);
    fm.appendUser(s2);

    // Delete a user
    if (admin.deleteUser(users, "student1")) {
        cout << "student1 deleted from system.\n";
        fm.saveAll(users); // Save updated list
    } else {
        cout << "User not found.\n";
    }

    // Display updated users
    cout << "\n=== Current Users ===\n";
    for (auto& u : users) {
        u.displayInfo();
        cout << "-----------------\n";
    }

    return 0;
}
