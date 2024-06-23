#pragma once

#include "event.h"

enum class ResStatus { Pending, Approved, Cancelled };

class Payment {
public:
    Payment(double amount);
    double getAmount() const;
    bool isPaid() const;
    void makePayment();

private:
    double amount;
    bool paid;
};

class Reservation {
public:
    Reservation(int id, User* user, Event* event, ResStatus status, Payment payment);

    int getId() const;
    User* getUser() const;
    Event* getEvent() const;
    ResStatus getStatus() const;
    const Payment& getPayment() const;
    void approve();
    void cancel();
    void makePayment();

    // Save reservation data to file
    void saveToFile(const string& filename) const;

    // Load reservation data from file
    static vector<Reservation> loadFromFile(const string& filename, const map<int, Event*>& eventMap, const map<string, User*>& userMap);

private:
    int id;
    User* user;
    Event* event;
    ResStatus status;
    Payment payment;
};
