#include "FileIO.h"

#include "logMessage.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <optional>

using namespace tinyxml2;
std::string FileIO::savePath = "";

static std::string savePath;
std::optional<std::string> FileIO::loadBasePathFromRcFile() {
    const char* homeDir = getenv("HOME");
    if (!homeDir) {
        std::cerr << "HOME environment variable not set\n";
        return std::nullopt;
    }

    std::string rcPath = std::string(homeDir) + "/.concertmanagerrc";
    std::ifstream infile(rcPath);
    if (!infile) {
        std::cerr << "Cannot open " << rcPath << "\n";
        return std::nullopt;
    }

    std::string line;
    while (std::getline(infile, line)) {
        // Supporta sia chiave=valore che direttamente una path
    	if (line.rfind("path=", 0) == 0) {
    	    return line.substr(std::string("path=").length());
        } else if (!line.empty() && line[0] != '#') { // questo trova la path se manca path=
            return line;
        }
    }

    std::cerr << "No valid basePathScores found in " << rcPath << "\n";
    return std::nullopt;
}

std::string FileIO::getSafeText(tinyxml2::XMLElement* elem) {
    if (!elem) return "";
    const char* txt = elem->GetText();
    return txt ? txt : "";
}

void FileIO::saveConcertsToXML(const std::vector<Concert> &concerts, const std::string &path) const
{
    XMLDocument doc;

    XMLDeclaration* decl = doc.NewDeclaration(R"(xml version="1.0" encoding="UTF-8")");
    doc.InsertFirstChild(decl);

    XMLElement* root = doc.NewElement("concerts");
    doc.InsertEndChild(root);

    for (const Concert& c : concerts) {
        XMLElement* concertElem = doc.NewElement("concert");

        auto append = [&](const char* name, const std::string& value) {
            XMLElement* e = doc.NewElement(name);
            e->SetText(value.c_str());
            concertElem->InsertEndChild(e);
        };

        append("title", c.getTitle());
        append("comment", c.getComment());
        append("todo", c.getToDo());
        append("timestamp", c.getTimestampAsString());

        // Places
        XMLElement* placesElem = doc.NewElement("places");
        for (const auto& p : c.getPlaces()) {
            XMLElement* e = doc.NewElement("place");
            e->SetText(p.c_str());
            placesElem->InsertEndChild(e);
        }
        concertElem->InsertEndChild(placesElem);

        // Dates
        XMLElement* datesElem = doc.NewElement("dates");
        for (const auto& d : c.getDatesAsString()) {
            XMLElement* e = doc.NewElement("date");
            e->SetText(d.c_str());
            datesElem->InsertEndChild(e);
        }
        concertElem->InsertEndChild(datesElem);

        // Musicians
        XMLElement* musiciansElem = doc.NewElement("musicians");
        for (const auto& m : c.getMusicians()) {
            XMLElement* mElem = doc.NewElement("musician");

            auto mfield = [&](const char* tag, const std::string& val) {
                XMLElement* e = doc.NewElement(tag);
                e->SetText(val.c_str());
                mElem->InsertEndChild(e);
            };

            mfield("name", m.getName());
            mfield("phone", m.getPhone());
            mfield("instrument", m.getInstrument());
            mfield("email", m.getMail());
            mfield("address", m.getAddress());

            XMLElement* g = doc.NewElement("gage");
            g->SetText(std::to_string(m.getGage()).c_str());
            mElem->InsertEndChild(g);

            // ✅ timestamp
            XMLElement* ts = doc.NewElement("timestamp");
            ts->SetText(m.getTimestampAsString().c_str());
            mElem->InsertEndChild(ts);

            musiciansElem->InsertEndChild(mElem);
        }
        concertElem->InsertEndChild(musiciansElem);

        // Program
        XMLElement* programElem = doc.NewElement("program");
        for (const auto& p : c.getProgram()) {
            XMLElement* pElem = doc.NewElement("piece");

            auto pelem = [&](const char* tag, const std::string& val) {
                XMLElement* e = doc.NewElement(tag);
                e->SetText(val.c_str());
                pElem->InsertEndChild(e);
            };

            pelem("composer", p.getComposer());
            pelem("title", p.getTitle());
            pelem("duration", intToString(p.getDuration()));
            pelem("choir", p.hasChoir() ? "1" : "0");
            pelem("singerPart", p.getSingerPart());
            pelem("instruments", p.getInstruments());

            // ✅ timestamp
            XMLElement* ts = doc.NewElement("timestamp");
            ts->SetText(p.getTimestampAsString().c_str());
            pElem->InsertEndChild(ts);

            programElem->InsertEndChild(pElem);
        }
        concertElem->InsertEndChild(programElem);

        // Rehearsals
        XMLElement* rehElem = doc.NewElement("rehearsals");
        for (const auto& r : c.getRehearsals()) {
            XMLElement* rElem = doc.NewElement("rehearsal");

            auto rfield = [&](const char* tag, const std::string& val) {
                XMLElement* e = doc.NewElement(tag);
                e->SetText(val.c_str());
                rElem->InsertEndChild(e);
            };

            rfield("date", dateToString(r.getDate()));
            rfield("start", r.getStartTime());
            rfield("place", r.getPlace());
            rfield("musicians", r.getMusicians());

            // ✅ timestamp
            XMLElement* ts = doc.NewElement("timestamp");
            ts->SetText(r.getTimestampAsString().c_str());
            rElem->InsertEndChild(ts);

            rehElem->InsertEndChild(rElem);
        }
        concertElem->InsertEndChild(rehElem);

        // Scores
        XMLElement* scoresElem = doc.NewElement("scores");
        for (const auto& score : c.getScores()) {
            XMLElement* sElem = doc.NewElement("score");

            XMLElement* pathElem = doc.NewElement("path");
            pathElem->SetText(score.getPath().c_str());
            sElem->InsertEndChild(pathElem);

            scoresElem->InsertEndChild(sElem);
        }
        concertElem->InsertEndChild(scoresElem);

        root->InsertEndChild(concertElem);
    }

    doc.SaveFile(path.c_str());
}


std::vector<Concert> FileIO::loadConcertsFromXML(const std::string& path)
{
    LOG_MSG("Loading concerts from XML");
    std::vector<Concert> concerts;

    XMLDocument doc;
    if (doc.LoadFile(path.c_str()) != XML_SUCCESS) {
        LOG_MSG("Could not open XML file " + path);
        return concerts;
    }

    XMLElement* root = doc.FirstChildElement("concerts");

    // Leggi l'attributo pathScores dalla root
    for (XMLElement* concertElem = root->FirstChildElement("concert");
         concertElem;
         concertElem = concertElem->NextSiblingElement("concert")) {

        Concert concert;

        concert.setTitle(getSafeText(concertElem->FirstChildElement("title")));
        concert.setComment(getSafeText(concertElem->FirstChildElement("comment")));
        concert.setToDo(getSafeText(concertElem->FirstChildElement("todo")));
        std::string tsStr = getSafeText(concertElem->FirstChildElement("timestamp"));
        if (!tsStr.empty()) {
                        try {
                            long long ts = std::stoll(tsStr);
                            concert.setTimestamp(ts);
                        } catch (...) {
                            LOG_MSG("Invalid timestamp format for concert: ");
                        }
                    }

        // Places
        std::vector<std::string> places;
        XMLElement* placesElem = concertElem->FirstChildElement("places");
        for (XMLElement* placeElem = placesElem ? placesElem->FirstChildElement("place") : nullptr;
             placeElem;
             placeElem = placeElem->NextSiblingElement("place")) {
            places.push_back(getSafeText(placeElem));
        }
        concert.setPlaces(places);

        // Dates
        std::vector<std::string> dates;
        XMLElement* datesElem = concertElem->FirstChildElement("dates");
        for (XMLElement* dateElem = datesElem ? datesElem->FirstChildElement("date") : nullptr;
             dateElem;
             dateElem = dateElem->NextSiblingElement("date")) {
            dates.push_back(getSafeText(dateElem));
        }
        concert.setDatesAsString(dates);

        // Musicians
        std::vector<Musician> musicians;
        XMLElement* musiciansElem = concertElem->FirstChildElement("musicians");
        for (XMLElement* mElem = musiciansElem ? musiciansElem->FirstChildElement("musician") : nullptr;
             mElem;
             mElem = mElem->NextSiblingElement("musician")) {

            Musician m;
            m.setName(getSafeText(mElem->FirstChildElement("name")));
            m.setPhone(getSafeText(mElem->FirstChildElement("phone")));
            m.setInstrument(getSafeText(mElem->FirstChildElement("instrument")));
            m.setMail(getSafeText(mElem->FirstChildElement("email")));
            m.setAddress(getSafeText(mElem->FirstChildElement("address")));

            try {
                m.setGage(std::stod(getSafeText(mElem->FirstChildElement("gage"))));
            } catch (...) {
                m.setGage(0.0);
                LOG_MSG("Invalid gage value, set to 0");
            }

            // Lettura del timestamp
            std::string tsStr = getSafeText(mElem->FirstChildElement("timestamp"));
            if (!tsStr.empty()) {
                try {
                    long long ts = std::stoll(tsStr);
                    m.setTimestamp(ts);
                } catch (...) {
                    LOG_MSG("Invalid timestamp format for musician: " + tsStr);
                }
            }

            musicians.push_back(m);
        }
        concert.setMusicians(musicians);


        // Program
        std::vector<MusicalPiece> program;
        XMLElement* progElem = concertElem->FirstChildElement("program");
        for (XMLElement* pieceElem = progElem ? progElem->FirstChildElement("piece") : nullptr;
             pieceElem;
             pieceElem = pieceElem->NextSiblingElement("piece")) {

            MusicalPiece p("", "", 0, false, "", "");
            p.setComposer(getSafeText(pieceElem->FirstChildElement("composer")));
            p.setTitle(getSafeText(pieceElem->FirstChildElement("title")));

            try {
                p.setDuration(stringToInt(getSafeText(pieceElem->FirstChildElement("duration"))));
            } catch (...) {
                LOG_MSG("Invalid or missing duration");
                p.setDuration(0);
            }

            p.setChoir(getSafeText(pieceElem->FirstChildElement("choir")) == "1");
            p.setSingerPart(getSafeText(pieceElem->FirstChildElement("singerPart")));
            p.setInstruments(getSafeText(pieceElem->FirstChildElement("instruments")));

            // Lettura del timestamp
            std::string tsStr = getSafeText(pieceElem->FirstChildElement("timestamp"));
            if (!tsStr.empty()) {
                try {
                    long long ts = std::stoll(tsStr);
                    p.setTimestamp(ts);
                } catch (...) {
                    LOG_MSG("Invalid timestamp format for musical piece: " + tsStr);
                }
            }

            program.push_back(p);
        }
        concert.setProgram(program);


        // Rehearsals
        std::vector<Rehearsal> rehearsals;
        XMLElement* rehearsalsElem = concertElem->FirstChildElement("rehearsals");
        for (XMLElement* rElem = rehearsalsElem ? rehearsalsElem->FirstChildElement("rehearsal") : nullptr;
             rElem;
             rElem = rElem->NextSiblingElement("rehearsal")) {

            std::tm date{};
            std::string dateStr = getSafeText(rElem->FirstChildElement("date"));
            if (!stringToDate(dateStr, date)) {
                LOG_MSG("Invalid rehearsal date format: " + dateStr);
                continue;
            }

            std::string tsStr = getSafeText(rElem->FirstChildElement("timestamp"));
            long long ts = 0;
            try {
                ts = std::stoll(tsStr);
            } catch (const std::exception& e) {
                LOG_MSG("Invalid timestamp format: " + tsStr);
                continue;
            }

            Rehearsal r(
                date,
                getSafeText(rElem->FirstChildElement("start")),
                getSafeText(rElem->FirstChildElement("place")),
                getSafeText(rElem->FirstChildElement("musicians")),
				ts
            );
            rehearsals.push_back(r);
        }
        concert.setRehearsals(rehearsals);

        // Scores
        std::vector<Score> scores;
        XMLElement* scoresElem = concertElem->FirstChildElement("scores");
        for (XMLElement* sElem = scoresElem ? scoresElem->FirstChildElement("score") : nullptr;
             sElem;
             sElem = sElem->NextSiblingElement("score")) {

            XMLElement* pathElem = sElem->FirstChildElement("path");
            if (pathElem && pathElem->GetText()) {
                Score s;
                s.setPath(pathElem->GetText());
                scores.push_back(s);
            } else {
                LOG_MSG("Invalid or missing <path> element in <score>");
            }
        }
        concert.setScores(scores);



        concerts.push_back(concert);
    }

    LOG_MSG("Finished loading concerts");
    return concerts;
}
