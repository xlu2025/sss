#include "event.h"

// Event class implementation
Event::Event(int id, const string& name, User* organizer, const string& startTime, const string& endTime, bool isPublic, LayoutType layout)
    : id(id), name(name), organizer(organizer), startTime(startTime), endTime(endTime), isPublic(isPublic), layout(layout) {}

int Event::getId() const {
    return id;
}

string Event::getName() const {
    return name;
}

User* Event::getOrganizer() const {
    return organizer;
}

string Event::getStartTime() const {
    return startTime;
}

string Event::getEndTime() const {
    return endTime;
}

bool Event::getIsPublic() const {
    return isPublic;
}

LayoutType Event::getLayout() const {
    return layout;
}

void Event::saveToFile(const string& filename) const {
    ofstream file(filename, ios::app); // Append mode
    if (file.is_open()) {
        file << id << "," << name << "," << organizer->getUsername() << ","
            << startTime << "," << endTime << "," << (isPublic ? "1" : "0") << ","
            << static_cast<int>(layout) << endl;
        file.close();
    }
    else {
        cerr << "Unable to open file for writing: " << filename << endl;
    }
}

vector<Event> Event::loadFromFile(const string& filename, const map<string, User*>& userMap) {
    vector<unique_ptr<User>> users;
    vector<Event> events;
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

            if (tokens.size() == 7) {
                int id = stoi(tokens[0]);
                string name = tokens[1];
                string organizerUsername = tokens[2];
                string startTime = tokens[3];
                string endTime = tokens[4];
                bool isPublic = (tokens[5] == "1");
                LayoutType layout = static_cast<LayoutType>(stoi(tokens[6]));

                User* organizer = nullptr;
                if (userMap.find(organizerUsername) != userMap.end()) {
                    organizer = userMap.at(organizerUsername);
                }

                events.push_back(Event(id, name, organizer, startTime, endTime, isPublic, layout));
            }
            else {
                cerr << "Invalid data format in file: " << filename << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "Unable to open file for reading: " << filename << endl;
    }
    return events;
}
