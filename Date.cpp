#include <string>
#include <ctime>
#include <sstream>
#include <unordered_set>
#include <regex>
#include "Date.h"

/**
 * @brief Verifica se una stringa rappresenta una data valida nel formato "gg/mm/aaaa".
 * 
 * @param date Puntatore a una stringa contenente la data da verificare.
 * @return true se la stringa è nel formato corretto e rappresenta una data valida, false altrimenti.
 */
bool Date::isValidDateFormat(const std::string *date)
{
    tm tm;
    if (strptime(date->c_str(), "%d/%m/%Y", &tm))
        return true;
    return false;
}

/**
 * @brief Controlla se una stringa è in un formato valido per l'aggiunta/sottrazione temporale (es. "+5 days", "-2w").
 * 
 * Il formato accettato è: [+/-][numero] [unità di tempo opzionale], dove l'unità può essere:
 * - d, day, days
 * - w, week, weeks
 * - m, month, months
 * - y, year, years
 * 
 * @param input Puntatore alla stringa da validare.
 * @return true se il formato è valido, false altrimenti.
 */
bool Date::isValidStringFormat(const std::string *input)
{
    std::regex formatRegex("^([-+]\\d+)\\s*(d|day|days|w|week|weeks|m|month|months|y|year|years)?$", std::regex_constants::icase);

    if (std::regex_match(*input, formatRegex))
    {
        return true;
    }

    return false;
}

/**
 * @brief Applica un'operazione di offset temporale a una data (`tm`), partendo da una stringa di input come "+5 days".
 * 
 * @param input Puntatore alla stringa che rappresenta lo spostamento temporale (es. "+3 months").
 * @param resultDate Puntatore alla struttura `tm` da modificare. La data verrà aggiornata direttamente.
 */
void Date::parseDateString(const std::string *input, tm *resultDate)
{
    int number = 0;
    std::string timeIndicator;
    std::stringstream ss(*input);
    while (!ss.eof())
    {
        if (isdigit(ss.peek()))
        {
            ss >> number;
        }
        else if (isalpha(ss.peek()))
        {
            ss >> timeIndicator;
            break;
        }
        else
        {
            ss.ignore();
        }
    }

    if (input->at(0) == '-')
        number = number * -1;

    // Converti l'indicatore di tempo in minuscolo
    for (char &c : timeIndicator)
    {
        c = tolower(c);
    }

    if (timeIndicator == "d" || timeIndicator == "day" || timeIndicator == "days")
    {
        resultDate->tm_mday += number;
    }
    else if (timeIndicator == "w" || timeIndicator == "week" || timeIndicator == "weeks")
    {
        resultDate->tm_mday += number * 7;
    }
    else if (timeIndicator == "m" || timeIndicator == "month" || timeIndicator == "months")
    {
        resultDate->tm_mon += number;
    }
    else if (timeIndicator == "y" || timeIndicator == "year" || timeIndicator == "years")
    {
        resultDate->tm_year += number;
    }
    std::mktime(resultDate);
}

/**
 * @brief Ottiene la data e ora correnti del sistema e la memorizza nella struttura `tm` fornita.
 * 
 * @param date Puntatore a una struttura `tm` dove verrà salvata la data corrente.
 */
void Date::now(tm *date)
{
    std::time_t now = std::time(0); // get time now
    tm *localTime = localtime(&now);
    *date = *localTime;
}

/**
 * @brief Converte una stringa di data nel formato "gg/mm/aaaa" in una struttura `tm`.
 * 
 * @param date Puntatore alla stringa contenente la data.
 * @param d Puntatore alla struttura `tm` che verrà popolata con i dati della data.
 */
void Date::convertDate(std::string *date, tm *d)
{
    strptime(date->c_str(), "%d/%m/%Y", d);
}

/**
 * @brief Confronta due date e determina se la prima è precedente alla seconda.
 * 
 * @param d1 Puntatore alla prima data (`tm`).
 * @param d2 Puntatore alla seconda data (`tm`).
 * @return true se `d1` è precedente a `d2`, false altrimenti.
 */
bool Date::isEarlier(tm *d1, tm *d2)
{
    return difftime(mktime(d1), mktime(d2)) < 0;
}

/**
 * @brief Converte un oggetto `time_t` in un intero lungo (`long int`).
 * 
 * @param t Puntatore a un oggetto `time_t`.
 * @return Valore `long int` corrispondente.
 */
int long Date::toLong(std::time_t *t)
{
    return static_cast<long int>(*t);
}

/**
 * @brief Converte una struttura `tm` in un intero lungo (`long int`) rappresentante il tempo.
 * 
 * @param t Puntatore alla struttura `tm` da convertire.
 * @return Valore `long int` del tempo equivalente.
 */
int long Date::toLong(tm *t)
{
    time_t time = {0};
    time = mktime(t);
    return toLong(&time);
}

