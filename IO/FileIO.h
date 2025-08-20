// File.h
#pragma once
#include "Concert.h"
#include "tinyxml2.h"
#include <vector>
#include <string>
#include <optional>

class FileIO {
public:
	static std::string savePath;
	static bool dryRun;
	void saveConcertsToXML(const std::vector<Concert> &concerts,
			const std::string &path) const;
	std::vector<Concert> loadConcertsFromXML(const std::string &filename);
	std::optional<std::string> loadBasePathFromRcFile();
	void loadScores(std::vector<std::string>& availablePaths);
private:
	std::string getSafeText(tinyxml2::XMLElement *elem);
};
