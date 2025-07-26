#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>

// Funzione interna che scrive il log
inline void logMessageImpl(const std::string& msg, const char* func, const char* file, int line) {
	const char* homeDir = getenv("HOME");
	if (homeDir == nullptr) {
	    std::cerr << "Error: HOME environment variable not found.\n";
	    exit(1);
	}
	std::string logPath = std::string(homeDir) + "/app.log";
    std::ofstream logFile(logPath, std::ios::app);
    if (!logFile.is_open()) return;

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));

    logFile << "[" << buf << "] "
            << file << ":" << line << " (" << func << ") - "
            << msg << std::endl;
}

// Macro da usare ovunque
#define LOG_MSG(msg) logMessageImpl(msg, __FUNCTION__, __FILE__, __LINE__)
