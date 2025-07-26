#pragma once

#include <string>
#include "Timestamped.h"

class Musician : public Timestamped {
private:
    std::string name;
    std::string phone;
    std::string instrument;
    std::string email;
    std::string address;
    double gage;

public:
    // Costruttori
    Musician();
    Musician(const std::string& name,
             const std::string& phone,
             const std::string& instrument,
             const std::string& email,
             const std::string& address,
             double gage,
             long long ts = 0); // opzionale timestamp

    // Getter
    const std::string& getName() const;
    const std::string& getPhone() const;
    const std::string& getInstrument() const;
    const std::string& getMail() const;
    const std::string& getAddress() const;
    double getGage() const;

    // Setter
    void setName(const std::string& newName);
    void setPhone(const std::string& newPhone);
    void setInstrument(const std::string& newInstrument);
    void setMail(const std::string& newEmail);
    void setAddress(const std::string& newAddress);
    void setGage(double newGage);

    bool isEmpty() const;

    // Output
    void print() const;
    std::string toString() const;
};
