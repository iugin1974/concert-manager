#include "Musician.h"
#include <iostream>

// Costruttori
Musician::Musician()
    : gage(0.0), Timestamped() {}

Musician::Musician(const std::string& name,
                   const std::string& phone,
                   const std::string& instrument,
                   const std::string& email,
                   const std::string& address,
                   double gage,
                   long long ts)
    : name(name), phone(phone), instrument(instrument),
      email(email), address(address), gage(gage),
      Timestamped(ts == 0 ? generateTimestamp() : ts) {}

// Getter
const std::string& Musician::getName() const       { return name; }
const std::string& Musician::getPhone() const      { return phone; }
const std::string& Musician::getInstrument() const { return instrument; }
const std::string& Musician::getMail() const       { return email; }
const std::string& Musician::getAddress() const    { return address; }
double Musician::getGage() const                   { return gage; }

// Setter (rigenerano il timestamp, perché "modifica")
void Musician::setName(const std::string& newName) {
    name = newName;
    regenerateTimestamp();
}
void Musician::setPhone(const std::string& newPhone) {
    phone = newPhone;
    regenerateTimestamp();
}
void Musician::setInstrument(const std::string& newInstrument) {
    instrument = newInstrument;
    regenerateTimestamp();
}
void Musician::setMail(const std::string& newEmail) {
    email = newEmail;
    regenerateTimestamp();
}
void Musician::setAddress(const std::string& newAddress) {
    address = newAddress;
    regenerateTimestamp();
}
void Musician::setGage(double newGage) {
    gage = newGage;
    regenerateTimestamp();
}

bool Musician::isEmpty() const {
    return name.empty();
}

// Print
void Musician::print() const {
    std::cout << "Nome: " << name << "\n"
              << "Strumento: " << instrument << "\n"
              << "Telefono: " << phone << "\n"
              << "Email: " << email << "\n"
              << "Indirizzo: " << address << "\n"
              << "Cachet: €" << gage << "\n"
              << "Timestamp: " << getTimestamp() << "\n";
}

std::string Musician::toString() const {
    return name;
}
