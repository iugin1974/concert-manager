#ifndef MUSICIAN_H
#define MUSICIAN_H

#include <string>

class Musician {
private:
    std::string name;
    std::string phone;
    std::string instrument;
    std::string email;
    std::string address;
    double gage;  // cachet

public:
    // Costruttore
	Musician();
    Musician(const std::string& name,
             const std::string& phone,
             const std::string& instrument,
             const std::string& email,
             const std::string& address,
             double gage);

    // Getter
    std::string getName() const;
    std::string getPhone() const;
    std::string getInstrument() const;
    std::string getMail() const;
    std::string getAddress() const;
    double getGage() const;

    // Setter
    void setName(const std::string& newName);
    void setPhone(const std::string& newPhone);
    void setInstrument(const std::string& newInstrument);
    void setMail(const std::string& newEmail);
    void setAddress(const std::string& newAddress);
    void setGage(double newGage);

	bool isEmpty();

    // Stampa su terminale
    void print() const;
};

#endif // MUSICIAN_H
