#include "CSV_Exporter.h"
#include "Musician.h"
#include <fstream>
#include <sstream>

void CSV_Exporter::exportCSV(const Concert &concert, const std::string &filename) {
	std::ofstream file(filename);
	if (!file.is_open()) return;

	// Header
	file << "Name;Mail;Phone;Address;ZIP;City;Gage\n";

	// Each musician
	for (const auto &m : concert.getMusicians()) {
		file << '"' << m.getName() << "\";"
		     << '"' << m.getMail() << "\";"
		     << '"' << m.getPhone() << "\";"
		     << '"' << m.getStreet() << "\";"
		     << '"' << m.getZipCode() << "\";"
		     << '"' << m.getCity() << "\";"
		     << '"' << m.getGage() << "\"\n";
	}

	file.close();
}
