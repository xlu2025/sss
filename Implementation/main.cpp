#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <ctime>
#include "facility_manager.h"

using namespace std;

// If make_unique is not available, implement it for C++11
#if __cplusplus == 201103L
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

void displayMainMenu() {
    cout << "\nCommunity Center Management System\n";
    cout << "1. Login\n";
    cout << "2. Create User\n";
    cout << "3. View Today's Schedule\n";
    cout << "4. View Weekly Schedule\n";
    cout << "5. Request Reservation\n";
    cout << "6. Cancel Reservation\n";
    cout << "7. View/Manage My Reservations\n";
    cout << "8. Logout\n";
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
}

void displayReservationMenu() {
    cout << "1. Create Event\n";
    cout << "2. Make Reservation\n";
    cout << "3. Approve Reservation\n";
    cout << "4. Go Back to Main Menu\n";
    cout << "Enter your choice: ";
}

string getCurrentDate() {
    // This function will return the current date in YYYY-MM-DD format
    time_t now = time(0);
    tm* ltm = localtime(&now); // Use localtime instead of localtime_s
    char date[11];
    snprintf(date, sizeof(date), "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(date);
}

int main() {
    // Initialize FacilityManager with paths to data files
    FacilityManager manager("users.txt", "events.txt", "reservations.txt");

    User* loggedInUser = nullptr;
    int mainChoice, subChoice;

    do {
        displayMainMenu();
        cin >> mainChoice;

        switch (mainChoice) {
        case 1: {
            // Login
            string username, password;
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter Password: ";
            cin >> password;

            auto& userMap = manager.getUserMap(); // Get a non-const reference
            if (userMap.find(username) != userMap.end() && userMap.at(username)->getPassword() == password) {
                loggedInUser = userMap.at(username);
                cout << "Login successful! Welcome, " << username << ".\n";
            }
            else {
                cout << "Invalid username or password. Please try again.\n";
            }
            break;
        }
        case 2: {
            // Create User
            int userType;
            string username, password;
            cout << "Select User Type: 1. City 2. Resident 3. Non-Resident 4. Organization\n";
            cin >> userType;
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter Password: ";
            cin >> password;

            unique_ptr<User> newUser;
            switch (userType) {
            case 1:
                newUser = make_unique<City>(username, password);
                break;
            case 2:
                newUser = make_unique<Resident>(username, password);
                break;
            case 3:
                newUser = make_unique<NonResident>(username, password);
                break;
            case 4:
                newUser = make_unique<Organization>(username, password);
                break;
            default:
                cout << "Invalid user type.\n";
                continue;
            }

            if (newUser) {
                auto& userMap = manager.getUserMap(); // Get a non-const reference
                userMap.emplace(username, newUser.get());
                newUser->saveToFile("users.txt");
                manager.getUsers().push_back(move(newUser));
                cout << "User created successfully.\n";
            }
            break;
        }
        case 3: {
            // View Today's Schedule
            string today = getCurrentDate();
            cout << "Today's Date: " << today << "\n";
            manager.viewDailySchedule(today);
            break;
        }
        case 4: {
            // View Weekly Schedule
            cout << "View Weekly Schedule (Starting from today).\n";
            string today = getCurrentDate();
            manager.viewWeeklySchedule(today);
            break;
        }
        case 5: {
            // Request Reservation
            if (!loggedInUser) {
                cout << "You need to login first.\n";
                break;
            }
            displayReservationMenu();
            cin >> subChoice;

            switch (subChoice) {
            case 1: {
                // Create Event
                string eventName, startTime, endTime;
                bool isPublic;
                int layoutType;
                cout << "Enter Event Name: ";
                cin >> eventName;
                cout << "Enter Start Time (YYYY-MM-DD HH:MM): ";
                cin >> startTime;
                cout << "Enter End Time (YYYY-MM-DD HH:MM): ";
                cin >> endTime;
                cout << "Is Public (1 for Yes, 0 for No): ";
                cin >> isPublic;
                cout << "Select Layout Type: 1. Meeting 2. Lecture 3. Wedding 4. Dance\n";
                cin >> layoutType;

                LayoutType layout = static_cast<LayoutType>(layoutType - 1);
                Event event(static_cast<int>(manager.getEvents().size()) + 1, eventName, loggedInUser, startTime, endTime, isPublic, layout);
                manager.addEvent(event);
                break;
            }
            case 2: {
                // Make Reservation
                int eventId;
                cout << "Enter Event ID: ";
                cin >> eventId;

                if (eventId < 0 || eventId >= static_cast<int>(manager.getEvents().size())) {
                    cout << "Invalid Event ID.\n";
                    break;
                }

                Reservation res(static_cast<int>(manager.getReservations().size()) + 1, loggedInUser, const_cast<Event*>(&manager.getEvents()[eventId]), ResStatus::Pending, Payment(loggedInUser->getRate()));
                manager.addReservation(res);
                break;
            }
            case 3: {
                // Approve Reservation (Admin Functionality)
                if (!loggedInUser || loggedInUser->getUserType() != UserType::City) {
                    cout << "Only city admin can approve reservations.\n";
                    break;
                }

                int resId;
                cout << "Enter Reservation ID to Approve: ";
                cin >> resId;

                if (resId < 0 || resId >= static_cast<int>(manager.getReservations().size())) {
                    cout << "Invalid Reservation ID.\n";
                    break;
                }

                manager.approveReservation(resId);
                // Make payment for the reservation
                Reservation& res = const_cast<Reservation&>(manager.getReservations()[resId]);
                res.makePayment();
                break;
            }
            case 4:
                // Go back to Main Menu
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
            break;
        }
        case 6: {
            // Cancel Reservation
            if (!loggedInUser) {
                cout << "You need to login first.\n";
                break;
            }
            int resId;
            cout << "Enter Reservation ID to Cancel: ";
            cin >> resId;

            if (resId < 0 || resId >= static_cast<int>(manager.getReservations().size())) {
                cout << "Invalid Reservation ID.\n";
                break;
            }

            manager.cancelReservation(resId);
            break;
        }
        case 7: {
            // View/Manage My Reservations
            if (!loggedInUser) {
                cout << "You need to login first.\n";
                break;
            }
            manager.viewUserReservations(loggedInUser);
            break;
        }
        case 8:
            // Logout
            loggedInUser = nullptr;
            cout << "Logged out successfully.\n";
            break;
        case 9:
            // Exit
            cout << "Saving data and exiting the system.\n";
            // Save events and reservations to files before exiting
            manager.saveEventsToFile("events.txt");
            manager.saveReservationsToFile("reservations.txt");
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (mainChoice != 9);

    return 0;
}
