#pragma once

#include "reservation.h"
#include <memory>
#include <map>

class FacilityManager {
public:
    FacilityManager(const string& usersFile = "users.txt", const string& eventsFile = "events.txt", const string& reservationsFile = "reservations.txt");

    void addEvent(const Event& event);
    void addReservation(const Reservation& reservation);
    void approveReservation(int resId);
    void cancelReservation(int resId);
    void viewDailySchedule(const string& date) const;
    void viewWeeklySchedule(const string& startDate) const;
    void viewUserReservations(User* user) const;

    // Save events and reservations to files
    void saveEventsToFile(const string& filename) const;
    void saveReservationsToFile(const string& filename) const;

    // Load users, events, and reservations from files
    void loadUsersFromFile(const string& filename);
    void loadEventsFromFile(const string& filename);
    void loadReservationsFromFile(const string& filename);

    // Access functions
    const vector<Event>& getEvents() const;
    const vector<Reservation>& getReservations() const;
    map<string, User*>& getUserMap(); // Changed to return non-const reference
    const map<int, Event*>& getEventMap() const;
    vector<unique_ptr<User>>& getUsers(); // 声明getUsers方法

private:
    vector<Event> events;
    vector<Reservation> reservations;
    vector<unique_ptr<User>> users; // 存储用户对象的容器
    map<int, Event*> eventMap;
    map<string, User*> userMap;

    void displayEvent(const Event& event) const;
    void displayReservation(const Reservation& res) const;
    string calculateEndDate(const string& startDate, int days) const;
};
