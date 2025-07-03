#include "Musician.h"
#include <iostream>

// Costruttore
Musician::Musician() {}
Musician::Musician(const std::string& name,
                   const std::string& phone,
                   const std::string& instrument,
                   const std::string& email,
                   const std::string& address,
                   double gage)
    : name(name), phone(phone), instrument(instrument),
      email(email), address(address), gage(gage) {}

// Getter
std::string Musician::getName() const       { return name; }
std::string Musician::getPhone() const      { return phone; }
std::string Musician::getInstrument() const { return instrument; }
std::string Musician::getMail() const       { return email; }
std::string Musician::getAddress() const    { return address; }
double Musician::getGage() const            { return gage; }

// Setter
void Musician::setName(const std::string& newName)             { name = newName; }
void Musician::setPhone(const std::string& newPhone)           { phone = newPhone; }
void Musician::setInstrument(const std::string& newInstrument) { instrument = newInstrument; }
void Musician::setMail(const std::string& newEmail)           { email = newEmail; }
void Musician::setAddress(const std::string& newAddress)       { address = newAddress; }
void Musician::setGage(double newGage)                         { gage = newGage; }

bool Musician::isEmpty() { return name.empty(); }
// Print
void Musician::print() const {
    std::cout << "Nome: " << name << "\n"
              << "Strumento: " << instrument << "\n"
              << "Telefono: " << phone << "\n"
              << "Email: " << email << "\n"
              << "Indirizzo: " << address << "\n"
              << "Cachet: €" << gage << "\n";
}
