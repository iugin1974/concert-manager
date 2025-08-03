#ifndef PRINTABLE_H
#define PRINTABLE_H

#include <string>
#include <sstream>
#include <iomanip>

class Printable {
public:
    virtual ~Printable() = default;

    // Restituisce la riga formattata con i dati dell'istanza
    virtual std::string toString() const = 0;
};

#endif // PRINTABLE_H
