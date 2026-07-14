#include "Model.h"
#include "logMessage.h"
#include <string>
#include <filesystem>
#include <algorithm> // std::sort, swap
#include <stdexcept> // per std::out_of_range
#include "FileIO.h"
#include "ftp.h"
#include "appconfig.h"
namespace fs = std::filesystem;

namespace {

struct FtpCredentials {
    std::string user;
    std::string host;
    std::string pass;
};

// Rilegge sempre da disco: niente cache, così load-da-file + save-su-FTP
// funziona a prescindere da come sono arrivati i dati in memoria.
bool readFtpCredentials(FtpCredentials& creds) {
    AppConfig::init("concertmanager");

    if (!AppConfig::read("user_ftp", &creds.user)) {
        std::cerr << "The " << AppConfig::configFilePath()
                  << " file was not found or the key 'user_ftp' does not exist.\n"
                  << "Edit the .concertmanagerrc file by adding the string 'user_ftp=<username>'\n";
        return false;
    }
    if (!AppConfig::read("host_ftp", &creds.host)) {
        std::cerr << "The " << AppConfig::configFilePath()
                  << " file was not found or the key 'host_ftp' does not exist.\n"
                  << "Edit the .concertmanagerrc file by adding the string 'host_ftp=<hostname>'\n";
        return false;
    }
    if (!AppConfig::read("pass_ftp", &creds.pass)) {
        std::cerr << "The " << AppConfig::configFilePath()
                  << " file was not found or the key 'pass_ftp' does not exist.\n"
                  << "Edit the .concertmanagerrc file by adding the string 'pass_ftp=<password>'\n";
        return false;
    }
    return true;
}

} // namespace

bool Model::loadFromFile(const std::string &filename) {
    FileIO f;
    concerts = f.loadConcertsFromXML(filename);
    return true; // TODO
}

bool Model::loadFromFTP(const std::string& filename) {
    FtpCredentials creds;
    if (!readFtpCredentials(creds)) return false;

    ftp client;
    client.setHost(creds.host);
    client.setUserName(creds.user);
    client.setPassword(creds.pass);

    std::string tmpFile = "/tmp/" + filename;
    if (client.downloadFile(filename, tmpFile) != 0) {
        std::cerr << "Download FTP failed\n";
        return false;
    }

    FileIO f;
    concerts = f.loadConcertsFromXML(tmpFile);
    return true;
}

bool Model::saveToFile(const std::string &filename) const {
    FileIO f;
    f.saveConcertsToXML(concerts, filename);
    return true; // TODO
}

bool Model::saveToFTP(const std::string& filename) const {
    FtpCredentials creds;
    if (!readFtpCredentials(creds)) return false;

    ftp client;
    client.setHost(creds.host);
    client.setUserName(creds.user);
    client.setPassword(creds.pass);

    std::string tmpFile = "/tmp/" + filename;
    FileIO f;
    f.saveConcertsToXML(concerts, tmpFile);

    if (client.uploadFile(tmpFile) != 0) {
        std::cerr << "Upload FTP failed\n";
        return false;
    }
    return true;
}


bool compareConcertByFirstDate(const Concert &a, const Concert &b) {
	const auto &datesA = a.getDatesAsTm();
	const auto &datesB = b.getDatesAsTm();

	if (datesA.empty() && datesB.empty())
		return false; // uguali, non cambia l'ordine
	if (datesA.empty())
		return false; // a senza date va dopo b
	if (datesB.empty())
		return true; // b senza date va dopo a

	// Confronta la prima data usando lo stesso criterio di prima
	const std::tm &dA = datesA.front();
	const std::tm &dB = datesB.front();

	if (dA.tm_year != dB.tm_year)
		return dA.tm_year < dB.tm_year;
	if (dA.tm_mon != dB.tm_mon)
		return dA.tm_mon < dB.tm_mon;
	return dA.tm_mday < dB.tm_mday;
}

void Model::sortConcerts() {
	std::sort(concerts.begin(), concerts.end(), compareConcertByFirstDate);
}


// Accesso ai concerti
const std::vector<Concert>& Model::getConcerts() const {
	return concerts;
}

std::vector<Concert>& Model::getConcerts() {
	return concerts;
}

Concert* Model::getConcert(int index) {
	if (index < 0 || static_cast<size_t>(index) >= concerts.size()) {
		throw std::out_of_range("Index out of range for vector 'concerts'");
	}

	return &concerts[index];
}

// Operazioni
void Model::addConcert(const Concert &concert) {
	concerts.push_back(concert);
}

void Model::updateConcertInfo(const Concert &source, Concert &target) {
	target.setTitle(source.getTitle());
	target.setPlaces(source.getPlaces());
	target.setDatesAsString(source.getDatesAsString());
	target.setStartTimesAsString(source.getStartTimesAsString());
}



void Model::addPiece(const MusicalPiece &piece, Concert &concert) {
	std::vector<MusicalPiece> &pieces = concert.getProgram();
	pieces.push_back(piece);
}

void Model::updatePiece(const MusicalPiece &oldPiece,
		const MusicalPiece &newPiece, Concert &concert) {
	std::vector<MusicalPiece> &pieces = concert.getProgram();
	for (MusicalPiece &p : pieces) {
		if (p.getTitle() == oldPiece.getTitle()
				&& p.getComposer() == oldPiece.getComposer()) {
			p = newPiece;
			return;
		}
	}
}

void Model::deleteConcert(Concert *concert) {
	for (auto it = concerts.begin(); it != concerts.end(); it++) {
		if (it->isSameAs(*concert)) {
			concerts.erase(it);
			return;
		}
	}
}

void Model::deleteConcert(size_t index) {
	if (index < concerts.size()) {
		concerts.erase(concerts.begin() + index);
	}
}

Rehearsal* Model::createEmptyRehearsal(Concert &concert) {
    concert.getRehearsals().emplace_back(); // Usa costruttore vuoto
    return &concert.getRehearsals().back();
}

Musician* Model::createEmptyMusician(Concert &concert) {
	concert.getMusicians().emplace_back();
	return &concert.getMusicians().back();
}

MusicalPiece* Model::createEmptyPiece(Concert &concert) {
	concert.getProgram().emplace_back();
	return &concert.getProgram().back();
}

void Model::removeRehearsal(Rehearsal* rehearsal, Concert &concert) {
    auto &rehearsals = concert.getRehearsals();
    rehearsals.erase(std::remove_if(rehearsals.begin(), rehearsals.end(),
        [&](const Rehearsal& r) { return &r == rehearsal; }),
        rehearsals.end());
}

void Model::removeMusician(Musician* musician, Concert &concert) {
	auto &musicians = concert.getMusicians();
	musicians.erase(std::remove_if(musicians.begin(), musicians.end(),
			[&](const Musician& m) { return &m == musician; }),
			musicians.end());
}

void Model::removeElement(MusicalPiece* piece, Concert &concert) {
	auto &program = concert.getProgram();
	program.erase(std::remove_if(program.begin(), program.end(),
			[&](const MusicalPiece& p) { return &p == piece; }),
			program.end());
}

void Model::updateRehearsal(const Rehearsal &oldRehearsal,
		const Rehearsal &newRehearsal, Concert &concert) {
	std::vector<Rehearsal> &rehearsals = concert.getRehearsals();
	for (Rehearsal &r : rehearsals) {
		if (r.isSameAs(oldRehearsal)) { // Presuppone un metodo isSameAs() in Rehearsal
			r = newRehearsal;
			return;
		}
	}
}

void Model::updateMusician(const Musician &oldM, const Musician &newM,
		Concert &concert) {
	std::vector<Musician> &musicians = concert.getMusicians();
	for (Musician &m : musicians) {
		if (m.isSameAs(oldM)) { // usa un confronto adeguato
			m = newM;
			return;
		}
	}
}

// aggiungi una score al concert
void Model::addScore(const Score &score, MusicalPiece &piece) {
	std::vector<Score> &scores = piece.getScores();
	scores.push_back(score);
}

// elimina una score dal concert
void Model::deleteScore(const Score &score, MusicalPiece &piece) {
	std::vector<Score> &scores = piece.getScores();
	for (auto it = scores.begin(); it != scores.end(); ++it) {
		if (it->getPath() == score.getPath()) {
			scores.erase(it);
			return;
		}
	}
}

void Model::addComment(const std::string &comment, Concert *concert) {
	concert->setComment(comment);
}

void Model::loadScorePaths() {
	FileIO f;
	f.loadScores(scorePaths);
	// Ordina alfabeticamente (lexicograficamente)
	std::sort(scorePaths.begin(), scorePaths.end(),
			[](const std::string &a, const std::string &b) {
				std::string fileA = fs::path(a).filename().string();
				std::string fileB = fs::path(b).filename().string();
				return fileA < fileB;
			});
}

const std::vector<std::string>& Model::getScorePaths() const {
	return scorePaths;
}

void Model::clear() {
	concerts.clear();
}

size_t Model::concertCount() const {
	return concerts.size();
}
