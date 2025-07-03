#ifndef MUSICALPIECE_H
#define MUSICALPIECE_H

#include <string>

class MusicalPiece {
private:
    std::string composer;
    std::string title;
    std::string key;
    std::string duration;     // formato MM:SS
    bool choir;
    std::string singerPart;
    std::string instruments;  // es. "Vl1, Vl2, Vla, Kb"

public:
    // Costruttore
    MusicalPiece(const std::string& composer,
                 const std::string& title,
                 const std::string& key,
                 const std::string& duration,
                 bool choir,
                 const std::string& singerPart,
                 const std::string& instruments);

    // Getter
    std::string getComposer() const;
    std::string getTitle() const;
    std::string getKey() const;
    std::string getDuration() const;
    bool hasChoir() const;
    std::string getSingerPart() const;
    std::string getInstruments() const;

    // Setter
    void setComposer(const std::string& composer);
    void setTitle(const std::string& title);
    void setKey(const std::string& key);
    void setDuration(const std::string& duration);
    void setChoir(bool choir);
    void setSingerPart(const std::string& singerPart);
    void setInstruments(const std::string& instruments);

    // Stampa a terminale
    void print() const;
};

#endif // MUSICALPIECE_H
