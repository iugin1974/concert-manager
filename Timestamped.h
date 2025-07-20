#pragma once
#include <chrono>
#include <string>

class Timestamped {
protected:
    long long timestamp;

    static long long generateTimestamp();

public:
    Timestamped();                    // genera automaticamente
    Timestamped(long long ts);        // usa timestamp fornito
    void setTimestamp(long long ts);

    long long getTimestamp() const;
    std::string getTimestampAsString() const;
    void regenerateTimestamp();       // rigenera il timestamp (es: oggetto modificato)
    bool isSameAs(const Timestamped& other) const;
};
