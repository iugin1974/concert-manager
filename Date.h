#ifndef DATE_H
#define DATE_H

#include <string>
#include <ctime>

/**
 * @brief Classe statica per la gestione e manipolazione di date.
 */
class Date
{
  public:
    /**
     * @brief Verifica se una stringa rappresenta una data valida nel formato "gg/mm/aaaa".
     * 
     * @param date Puntatore a una stringa contenente la data da verificare.
     * @return true se la stringa è nel formato corretto e rappresenta una data valida, false altrimenti.
     */
    static bool isValidDateFormat(const std::string *date);

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
    static bool isValidStringFormat(const std::string* input);

    /**
     * @brief Applica un'operazione di offset temporale a una data (`tm`), partendo da una stringa di input come "+5 days".
     * 
     * @param input Puntatore alla stringa che rappresenta lo spostamento temporale (es. "+3 months").
     * @param resultDate Puntatore alla struttura `tm` da modificare. La data verrà aggiornata direttamente.
     */
    static void parseDateString(const std::string* input, tm* resultDate);

    /**
     * @brief Confronta due date e determina se la prima è precedente alla seconda.
     * 
     * @param d1 Puntatore alla prima data (`tm`).
     * @param d2 Puntatore alla seconda data (`tm`).
     * @return true se `d1` è precedente a `d2`, false altrimenti.
     */
    static bool isEarlier(tm *d1, tm *d2);

    /**
     * @brief Converte una stringa di data nel formato "gg/mm/aaaa" in una struttura `tm`.
     * 
     * @param date Puntatore alla stringa contenente la data.
     * @param d Puntatore alla struttura `tm` che verrà popolata con i dati della data.
     */
    static void convertDate(std::string *date, tm *d);

    /**
     * @brief Ottiene la data e ora correnti del sistema e la memorizza nella struttura `tm` fornita.
     * 
     * @param date Puntatore a una struttura `tm` dove verrà salvata la data corrente.
     */
    static void now(tm* date);

    /**
     * @brief Converte un oggetto `time_t` in un intero lungo (`long int`).
     * 
     * @param t Puntatore a un oggetto `time_t`.
     * @return Valore `long int` corrispondente.
     */
    static int long toLong(std::time_t *t);

    /**
     * @brief Converte una struttura `tm` in un intero lungo (`long int`) rappresentante il tempo.
     * 
     * @param t Puntatore alla struttura `tm` da convertire.
     * @return Valore `long int` del tempo equivalente.
     */
    static int long toLong(tm* t);
};

#endif

