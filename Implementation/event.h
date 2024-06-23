#pragma once

#include "user.h"

using namespace std;

enum class LayoutType { Meeting, Lecture, Wedding, Dance };

class Event {
public:
    Event(int id, const string& name, User* organizer, const string& startTime, const string& endTime, bool isPublic, LayoutType layout);

    int getId() const;
    string getName() const;
    User* getOrganizer() const;
    string getStartTime() const;
    string getEndTime() const;
    bool getIsPublic() const;
    LayoutType getLayout() const;

    // Save event data to file
    void saveToFile(const string& filename) const;

    // Load event data from file
    static vector<Event> loadFromFile(const string& filename, const map<string, User*>& userMap);

private:
    int id;
    string name;
    User* organizer;
    string startTime;
    string endTime;
    bool isPublic;
    LayoutType layout;
};
