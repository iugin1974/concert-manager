#include "Musician.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// Costruttori
Musician::Musician()
    : Timestamped(), gage(0.0) {}

Musician::Musician(const std::string &name, const std::string &phone,
                   const std::string &instrument, const std::string &email,
                   const std::string &street, const std::string &zipCode, const std::string &city,
                   double gage, int rehearsalNumber, int concertNumber,
                   bool soloist, double travelCosts, long long ts)
    : Timestamped(ts == 0 ? generateTimestamp() : ts),
      name(name),
      phone(phone),
      instrument(instrument),
      email(email),
      street(street),
      zipCode(zipCode),
      city(city),
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
const std::string& Musician::getStreet() const     { return street; }
const std::string& Musician::getZipCode() const        { return zipCode; }
const std::string& Musician::getCity() const       { return city; }

std::string Musician::getFullAddress() const {
    std::ostringstream oss;
    oss << street << "\n" << zipCode << " " << city;
    return oss.str();
}

double Musician::getGage() const                   { return gage; }
bool Musician::isSoloist() const                   { return soloist; }

int Musician::getRehearsalNumber() const           { return rehearsalNumber; }
int Musician::getConcertNumber() const             { return concertNumber; }
double Musician::getTravelCosts() const            { return travelCosts; }

// Setter (rigenerano il timestamp)
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

void Musician::setStreet(const std::string& s) {
    street = s;
    regenerateTimestamp();
}

void Musician::setZipCode(const std::string& z) {
    zipCode = z;
    regenerateTimestamp();
}

void Musician::setCity(const std::string& c) {
    city = c;
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

void Musician::setSoloist(bool s) {
    soloist = s;
    regenerateTimestamp();
}

bool Musician::isEmpty() const {
    return name.empty();
}

double Musician::getFullPayment() const {
    double travel = getTravelCosts() * ((getRehearsalNumber() * 2) + (getConcertNumber() * 2));
    return getGage() + travel;
}

// Output
std::string Musician::toString() const {
    std::ostringstream oss;
    oss << std::left
        << std::setw(22) << getName()
        << std::setw(20) << getInstrument()
        << std::right
        << std::setw(10) << std::fixed << std::setprecision(2) << getGage()
        << std::setw(25) << std::fixed << std::setprecision(2) << getFullPayment();
    return oss.str();
}

std::string Musician::header() {
    std::ostringstream oss;
    oss << std::left
        << std::setw(22) << "Name"
        << std::setw(20) << "Instrument"
        << std::right
        << std::setw(10) << "Gage"
        << std::setw(25) << "Total with expenses";
    return oss.str();
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
