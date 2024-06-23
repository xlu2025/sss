#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <map>

using namespace std;

enum class UserType {
    City,
    Resident,
    NonResident,
    Organization
};

class User {
public:
    User(const string& username, const string& password, UserType userType, double rate);
    virtual ~User() = default;

    string getUsername() const;
    string getPassword() const;
    UserType getUserType() const;
    double getRate() const;

    // Save user data to file
    void saveToFile(const string& filename) const;

    // Load user data from file
    static vector<unique_ptr<User>> loadFromFile(const string& filename, map<string, User*>& userMap);

private:
    string username;
    string password;
    UserType userType;
    double rate;
};

class City : public User {
public:
    City(const string& username, const string& password);
};

class Resident : public User {
public:
    Resident(const string& username, const string& password);
};

class NonResident : public User {
public:
    NonResident(const string& username, const string& password);
};

class Organization : public User {
public:
    Organization(const string& username, const string& password);
};
