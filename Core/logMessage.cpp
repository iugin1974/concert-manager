#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

void logMessage(const std::string& msg) {
    std::ofstream logFile("app.log", std::ios::app);
    if (!logFile.is_open()) return;

    // Timestamp semplice
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    // Formatta data e ora
    std::tm* tm_ptr = std::localtime(&now_c);
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_ptr);

    logFile << "[" << timeStr << "] " << msg << std::endl;
}

