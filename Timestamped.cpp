#include "Timestamped.h"
#include <string>

long long Timestamped::generateTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}

Timestamped::Timestamped() {
    timestamp = generateTimestamp();
}

Timestamped::Timestamped(long long ts) : timestamp(ts) {}

long long Timestamped::getTimestamp() const {
    return timestamp;
}

std::string Timestamped::getTimestampAsString() const {
    return std::to_string(timestamp);
}

void Timestamped::setTimestamp(long long ts) {
    timestamp = ts;
}

void Timestamped::regenerateTimestamp() {
    timestamp = generateTimestamp();
}

bool Timestamped::isSameAs(const Timestamped& other) const {
		return this->timestamp == other.timestamp;
	}
