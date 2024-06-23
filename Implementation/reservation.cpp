#include "reservation.h"

// Payment class implementation
Payment::Payment(double amount) : amount(amount), paid(false) {}

double Payment::getAmount() const {
    return amount;
}

bool Payment::isPaid() const {
    return paid;
}

void Payment::makePayment() {
    paid = true;
}

// Reservation class implementation
Reservation::Reservation(int id, User* user, Event* event, ResStatus status, Payment payment)
    : id(id), user(user), event(event), status(status), payment(payment) {}

int Reservation::getId() const {
    return id;
}

User* Reservation::getUser() const {
    return user;
}

Event* Reservation::getEvent() const {
    return event;
}

ResStatus Reservation::getStatus() const {
    return status;
}

const Payment& Reservation::getPayment() const {
    return payment;
}

void Reservation::approve() {
    status = ResStatus::Approved;
}

void Reservation::cancel() {
    status = ResStatus::Cancelled;
}

void Reservation::makePayment() {
    payment.makePayment();
}

void Reservation::saveToFile(const string& filename) const {
    ofstream file(filename, ios::app); // Append mode
    if (file.is_open()) {
        file << id << "," << user->getUsername() << "," << event->getId() << ","
            << static_cast<int>(status) << "," << payment.getAmount() << ","
            << (payment.isPaid() ? "1" : "0") << endl;
        file.close();
    }
    else {
        cerr << "Unable to open file for writing: " << filename << endl;
    }
}

vector<Reservation> Reservation::loadFromFile(const string& filename, const map<int, Event*>& eventMap, const map<string, User*>& userMap) {
    vector<Reservation> reservations;
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

            if (tokens.size() == 6) {
                int id = stoi(tokens[0]);
                string username = tokens[1];
                int eventId = stoi(tokens[2]);
                ResStatus status = static_cast<ResStatus>(stoi(tokens[3]));
                double amount = stod(tokens[4]);
                bool paid = (tokens[5] == "1");

                User* user = nullptr;
                Event* event = nullptr;

                if (userMap.find(username) != userMap.end()) {
                    user = userMap.at(username);
                }
                if (eventMap.find(eventId) != eventMap.end()) {
                    event = eventMap.at(eventId);
                }

                Payment payment(amount);
                if (paid) {
                    payment.makePayment();
                }

                reservations.push_back(Reservation(id, user, event, status, payment));
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
    return reservations;
}
