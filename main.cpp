#include "ConcertController.h"
#include "MainMenuView.h"
#include <vector>
#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <cstdlib>

#include "logMessage.h"
void saveConcertsToXML(const std::vector<Concert>& concerts) {
    std::string path = std::string(std::getenv("HOME")) + "/concerti.xml";
    std::ofstream file(path);
    if (!file) return;

    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<concerts>\n";

    for (const auto& concert : concerts) {
        file << "  <concert>\n";
        file << "    <title>" << concert.getTitle() << "</title>\n";

        file << "    <places>\n";
        for (const auto& place : concert.getPlaces())
            file << "      <place>" << place << "</place>\n";
        file << "    </places>\n";

        file << "    <dates>\n";
        for (const auto& date : concert.getDates())
            file << "      <date>" << date << "</date>\n";
        file << "    </dates>\n";

        file << "    <musicians>\n";
        for (const auto& m : concert.getMusicians()) {
            file << "      <musician>\n";
            file << "        <name>" << m.getName() << "</name>\n";
            file << "        <phone>" << m.getPhone() << "</phone>\n";
            file << "        <instrument>" << m.getInstrument() << "</instrument>\n";
            file << "        <email>" << m.getMail() << "</email>\n";
            file << "        <address>" << m.getAddress() << "</address>\n";
            file << "        <gage>" << m.getGage() << "</gage>\n";
            file << "      </musician>\n";
        }
        file << "    </musicians>\n";

        file << "    <program>\n";
        for (const auto& p : concert.getProgram()) {
            file << "      <piece>\n";
            file << "        <composer>" << p.getComposer() << "</composer>\n";
            file << "        <title>" << p.getTitle() << "</title>\n";
            file << "        <key>" << p.getKey() << "</key>\n";
            file << "        <duration>" << p.getDuration() << "</duration>\n";
            file << "        <choir>" << (p.hasChoir() ? "1" : "0") << "</choir>\n";
            file << "        <singerPart>" << p.getSingerPart() << "</singerPart>\n";
            file << "        <instruments>" << p.getInstruments() << "</instruments>\n";
            file << "      </piece>\n";
        }
        file << "    </program>\n";

        file << "  </concert>\n";
    }

    file << "</concerts>\n";
}

std::string getTagValue(const std::string& line, const std::string& tag) {
    auto start = line.find("<" + tag + ">");
    auto end = line.find("</" + tag + ">");
    if (start == std::string::npos || end == std::string::npos) return "";
    start += tag.length() + 2;
    return line.substr(start, end - start);
}

std::vector<Concert> loadConcertsFromXML() {
LOG_MSG("Load");
    std::vector<Concert> concerts;
    std::ifstream file(std::string(std::getenv("HOME")) + "/concerti.xml");
    if (!file) return concerts;

    std::string line;
    Concert currentConcert;
    std::vector<std::string> places, dates;
    std::vector<Musician> musicians;
    std::vector<MusicalPiece> program;

    while (std::getline(file, line)) {
        if (line.find("<concert>") != std::string::npos) {
            currentConcert = Concert();
            places.clear(); dates.clear();
            musicians.clear(); program.clear();
        } else if (line.find("<title>") != std::string::npos) {
            currentConcert.setTitle(getTagValue(line, "title"));
        } else if (line.find("<place>") != std::string::npos) {
            places.push_back(getTagValue(line, "place"));
        } else if (line.find("<date>") != std::string::npos) {
            dates.push_back(getTagValue(line, "date"));
        } else if (line.find("<musician>") != std::string::npos) {
            Musician m;
            std::getline(file, line); m.setName(getTagValue(line, "name"));
            std::getline(file, line); m.setPhone(getTagValue(line, "phone"));
            std::getline(file, line); m.setInstrument(getTagValue(line, "instrument"));
            std::getline(file, line); m.setMail(getTagValue(line, "email"));
            std::getline(file, line); m.setAddress(getTagValue(line, "address"));
            std::getline(file, line); m.setGage(std::stod(getTagValue(line, "gage")));
            musicians.push_back(m);
        } else if (line.find("<piece>") != std::string::npos) {
            MusicalPiece p("", "", "", "", false, "", "");
            std::getline(file, line); p.setComposer(getTagValue(line, "composer"));
            std::getline(file, line); p.setTitle(getTagValue(line, "title"));
            std::getline(file, line); p.setKey(getTagValue(line, "key"));
            std::getline(file, line); p.setDuration(getTagValue(line, "duration"));
            std::getline(file, line); p.setChoir(getTagValue(line, "choir") == "1");
            std::getline(file, line); p.setSingerPart(getTagValue(line, "singerPart"));
            std::getline(file, line); p.setInstruments(getTagValue(line, "instruments"));
            program.push_back(p);
        } else if (line.find("</concert>") != std::string::npos) {
            currentConcert.setPlaces(places);
            currentConcert.setDates(dates);
            currentConcert.setMusicians(musicians);
            currentConcert.setProgram(program);
            concerts.push_back(currentConcert);
        }
    }

    return concerts;
}

int main()
{
    std::vector<Concert> concerts = loadConcertsFromXML();  // carica
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    ConcertController controller;

    while (true)
    {
        MainMenuView menu;
        MainMenuView::Action action = menu.show();

        switch (action)
        {
        case MainMenuView::ADD_CONCERT: {
            auto concert = controller.createConcert();
if (concert) concerts.push_back(*concert);
}
            break;
        case MainMenuView::VIEW_CONCERTS:
            controller.listConcerts(concerts);
            break;
        case MainMenuView::EDIT_CONCERT:
            controller.editConcert(concerts);
            break;
        case MainMenuView::DELETE_CONCERT:
            controller.deleteConcert(concerts);
            break;
        case MainMenuView::EXIT:
    saveConcertsToXML(concerts);  // salva prima di uscire
    endwin();
    return 0;
        }
    }
}
