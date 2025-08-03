#pragma once

#include <string>
#include "Timestamped.h"
#include "Printable.h"

class Musician: public Timestamped, Printable {
private:
	std::string name;
	std::string phone;
	std::string instrument;
	std::string email;
	std::string address;
	int rehearsalNumber = 0;
	int concertNumber = 0;
	double travelCosts = 0.0;
	double gage;
	bool soloist = false;

public:
	// Costruttori
	Musician();
	Musician(const std::string &name, const std::string &phone,
	         const std::string &instrument, const std::string &email,
	         const std::string &address, double gage,
	         int rehearsalNumber = 0, int concertNumber = 0,
	         bool soloist = false, double travelCosts = 0.0,
	         long long ts = 0);
	// Getter
	const std::string& getName() const;
	const std::string& getPhone() const;
	const std::string& getInstrument() const;
	const std::string& getMail() const;
	const std::string& getAddress() const;
	double getGage() const;
	int getRehearsalNumber() const;
	int getConcertNumber() const;
	double getTravelCosts() const;
	bool isSoloist() const;
	double getFullPayment() const;
	struct SalaryDetails {
		double baseSalary;
		double vacationCompensation;
		double insurances;
		double travelCosts;
		double totalSalary;
	};

	static SalaryDetails calculateSalary(int rehearsals, int concerts,
			bool soloist, double travelCosts);

	// Setter
	void setName(const std::string &newName);
	void setPhone(const std::string &newPhone);
	void setInstrument(const std::string &newInstrument);
	void setMail(const std::string &newEmail);
	void setAddress(const std::string &newAddress);
	void setGage(double newGage);
	void setRehearsalNumber(int number);
	void setConcertNumber(int number);
	void setTravelCosts(double costs);
	void setSoloist(bool soloist);
	bool isEmpty() const;

	// Output
	void print() const;
	std::string toString() const;
	static std::string header();
};
