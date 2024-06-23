#include "user.h"
#include <fstream>
#include <sstream>
#include <memory>

// Manually define make unique to support C++11
#if __cplusplus == 201103L
#include <memory>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

using namespace std;

User::User(const string& username, const string& password, UserType userType, double rate)
    : username(username), password(password), userType(userType), rate(rate) {}

string User::getUsername() const {
    return username;
}

string User::getPassword() const {
    return password;
}

UserType User::getUserType() const {
    return userType;
}

double User::getRate() const {
    return rate;
}

void User::saveToFile(const string& filename) const {
    ofstream file(filename, ios::app); // Append mode
    if (file.is_open()) {
        file << username << "," << password << "," << static_cast<int>(userType) << "," << rate << endl;
        file.close();
    }
    else {
        cerr << "Unable to open file for writing: " << filename << endl;
    }
}

vector<unique_ptr<User>> User::loadFromFile(const string& filename, map<string, User*>& userMap) {
    vector<unique_ptr<User>> users;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t pos = 0;
            string token;
            vector<string> tokens;
            while ((pos = line.find(',')) != string::npos) {
                token = line.substr(0, pos);
                tokens.push_back(token);
                line.erase(0, pos + 1);
            }
            tokens.push_back(line);

            if (tokens.size() == 3) {
                string username = tokens[0];
                string password = tokens[1];
                UserType userType = static_cast<UserType>(stoi(tokens[2]));
                //rate = stod(tokens[3]); // Use rate in user construction

                unique_ptr<User> user;
                switch (userType) {
                case UserType::City:
                    user = make_unique<City>(username, password);
                    break;
                case UserType::Resident:
                    user = make_unique<Resident>(username, password);
                    break;
                case UserType::NonResident:
                    user = make_unique<NonResident>(username, password);
                    break;
                case UserType::Organization:
                    user = make_unique<Organization>(username, password);
                    break;
                }

                if (user) {
                    userMap[username] = user.get();
                    users.push_back(move(user));
                }
            }
        }
        file.close();
    }
    else {
        cerr << "Unable to open file: " << filename << endl;
    }
    return users;
}

// City class implementation
City::City(const string& username, const string& password)
    : User(username, password, UserType::City, 5.0) {}

// Resident class implementation
Resident::Resident(const string& username, const string& password)
    : User(username, password, UserType::Resident, 10.0) {}

// NonResident class implementation
NonResident::NonResident(const string& username, const string& password)
    : User(username, password, UserType::NonResident, 15.0) {}

// Organization class implementation
Organization::Organization(const string& username, const string& password)
    : User(username, password, UserType::Organization, 20.0) {}
