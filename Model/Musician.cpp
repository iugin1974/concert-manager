#include "Musician.h"
#include <iostream>

// Costruttori
Musician::Musician()
    : Timestamped(), gage (0.0) {}

Musician::Musician(const std::string &name, const std::string &phone,
                   const std::string &instrument, const std::string &email,
                   const std::string &address, double gage,
                   int rehearsalNumber, int concertNumber,
                   bool soloist, double travelCosts,
                   long long ts)
    : Timestamped(ts == 0 ? generateTimestamp() : ts),
      name(name),
      phone(phone),
      instrument(instrument),
      email(email),
      address(address),
      rehearsalNumber(rehearsalNumber),
      concertNumber(concertNumber),
      travelCosts(travelCosts),
      gage(gage),
      soloist(soloist)
{}

// Getter
const std::string& Musician::getName() const       { return name; }
const std::string& Musician::getPhone() const      { return phone; }
const std::string& Musician::getInstrument() const { return instrument; }
const std::string& Musician::getMail() const       { return email; }
const std::string& Musician::getAddress() const    { return address; }
double Musician::getGage() const                   { return gage; }
bool Musician::isSoloist() const				   { return soloist; }
int Musician::getRehearsalNumber() const {
    return rehearsalNumber;
}

int Musician::getConcertNumber() const {
    return concertNumber;
}

double Musician::getTravelCosts() const {
    return travelCosts;
}

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

void Musician::setRehearsalNumber(int number) {
    rehearsalNumber = number;
    regenerateTimestamp();
}

void Musician::setConcertNumber(int number) {
    concertNumber = number;
    regenerateTimestamp();
}

void Musician::setTravelCosts(double costs) {
    travelCosts = costs;
    regenerateTimestamp();
}

void Musician::setSoloist(bool soloist) {
	this->soloist = soloist;
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

Musician::SalaryDetails Musician::calculateSalary(int rehearsals, int concerts, bool soloist, double travelC) {
    SalaryDetails details;
    details.baseSalary = rehearsals * 175 + concerts * 203 + (soloist ? 100 : 0);
    details.vacationCompensation = details.baseSalary * 8.33 / 100;
    details.insurances = details.baseSalary * 6.25 / 100;
    details.totalSalary = details.baseSalary + details.vacationCompensation + details.insurances;
    details.travelCosts = travelC * ((rehearsals * 2) + (concerts * 2));
    return details;
}

