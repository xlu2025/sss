#include "facility_manager.h"

#include "facility_manager.h"

FacilityManager::FacilityManager(const string& usersFile, const string& eventsFile, const string& reservationsFile) {
    // Load users first
    loadUsersFromFile(usersFile);
    // Load events and populate eventMap
    loadEventsFromFile(eventsFile);
    // Load reservations
    loadReservationsFromFile(reservationsFile);
}

void FacilityManager::loadUsersFromFile(const string& filename) {
    // Load users from file and populate userMap
    users = User::loadFromFile(filename, userMap);
    for (auto& user : users) {
        userMap[user->getUsername()] = user.get();
    }
}

void FacilityManager::loadEventsFromFile(const string& filename) {
    // Load events from file and populate eventMap
    events = Event::loadFromFile(filename, userMap);
    for (auto& event : events) {
        eventMap[event.getId()] = &event;
    }
}

void FacilityManager::loadReservationsFromFile(const string& filename) {
    // Load reservations from file
    reservations = Reservation::loadFromFile(filename, eventMap, userMap);
}

// Other methods remain the same...

void FacilityManager::addEvent(const Event& event) {
    events.push_back(event);
    eventMap[event.getId()] = &events.back();
}

void FacilityManager::addReservation(const Reservation& reservation) {
    reservations.push_back(reservation);
}

void FacilityManager::approveReservation(int resId) {
    for (auto& res : reservations) {
        if (res.getId() == resId) {
            res.approve();
            return;
        }
    }
}

void FacilityManager::cancelReservation(int resId) {
    for (auto& res : reservations) {
        if (res.getId() == resId) {
            res.cancel();
            return;
        }
    }
}

void FacilityManager::viewDailySchedule(const string& date) const {
    for (const auto& event : events) {
        if (event.getStartTime().substr(0, 10) == date) {
            displayEvent(event);
        }
    }
}

void FacilityManager::viewWeeklySchedule(const string& startDate) const {
    string endDate = calculateEndDate(startDate, 7);
    for (const auto& event : events) {
        if (event.getStartTime().substr(0, 10) >= startDate && event.getStartTime().substr(0, 10) <= endDate) {
            displayEvent(event);
        }
    }
}

void FacilityManager::viewUserReservations(User* user) const {
    for (const auto& res : reservations) {
        if (res.getUser() == user) {
            displayReservation(res);
        }
    }
}

void FacilityManager::saveEventsToFile(const string& filename) const {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& event : events) {
            event.saveToFile(filename);
        }
        file.close();
    }
    else {
        cerr << "Unable to open file for writing: " << filename << endl;
    }
}

void FacilityManager::saveReservationsToFile(const string& filename) const {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& res : reservations) {
            res.saveToFile(filename);
        }
        file.close();
    }
    else {
        cerr << "Unable to open file for writing: " << filename << endl;
    }
}

void FacilityManager::displayEvent(const Event& event) const {
    cout << "Event ID: " << event.getId() << ", Name: " << event.getName()
        << ", Organizer: " << event.getOrganizer()->getUsername()
        << ", Start Time: " << event.getStartTime()
        << ", End Time: " << event.getEndTime()
        << ", Public: " << (event.getIsPublic() ? "Yes" : "No")
        << ", Layout: " << static_cast<int>(event.getLayout()) << "\n";
}

void FacilityManager::displayReservation(const Reservation& res) const {
    cout << "Reservation ID: " << res.getId()
        << ", User: " << res.getUser()->getUsername()
        << ", Event: " << res.getEvent()->getName()
        << ", Status: " << static_cast<int>(res.getStatus())
        << ", Paid: " << (res.getPayment().isPaid() ? "Yes" : "No") << "\n";
}

string FacilityManager::calculateEndDate(const string& startDate, int days) const {
    // Simple implementation assuming startDate is in "YYYY-MM-DD" format
    int year = stoi(startDate.substr(0, 4));
    int month = stoi(startDate.substr(5, 2));
    int day = stoi(startDate.substr(8, 2)) + days;
    return to_string(year) + "-" + (month < 10 ? "0" : "") + to_string(month) + "-" + (day < 10 ? "0" : "") + to_string(day);
}

const vector<Event>& FacilityManager::getEvents() const {
    return events;
}

const vector<Reservation>& FacilityManager::getReservations() const {
    return reservations;
}

map<string, User*>& FacilityManager::getUserMap() { // Changed to return non-const reference
    return userMap;
}

const map<int, Event*>& FacilityManager::getEventMap() const {
    return eventMap;
}

vector<unique_ptr<User>>& FacilityManager::getUsers() {
    return users;
}