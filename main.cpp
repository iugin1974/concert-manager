#include "ConcertController.h"
#include "MainMenuView.h"
#include <vector>
#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <cstdlib>

#include "logMessage.h"

std::string xmlEscape(const std::string &data)
{
    std::string escaped;
    for (char c : data)
    {
        switch (c)
        {
        case '&':
            escaped.append("&amp;");
            break;
        case '<':
            escaped.append("&lt;");
            break;
        case '>':
            escaped.append("&gt;");
            break;
        case '"':
            escaped.append("&quot;");
            break;
        case '\'':
            escaped.append("&apos;");
            break;
        default:
            escaped.push_back(c);
            break;
        }
    }
    return escaped;
}

std::string xmlUnescape(const std::string &data)
{
    std::string unescaped;
    for (size_t i = 0; i < data.size(); ++i)
    {
        if (data[i] == '&')
        {
            if (data.compare(i, 5, "&amp;") == 0)
            {
                unescaped.push_back('&');
                i += 4;
            }
            else if (data.compare(i, 4, "&lt;") == 0)
            {
                unescaped.push_back('<');
                i += 3;
            }
            else if (data.compare(i, 4, "&gt;") == 0)
            {
                unescaped.push_back('>');
                i += 3;
            }
            else if (data.compare(i, 6, "&quot;") == 0)
            {
                unescaped.push_back('"');
                i += 5;
            }
            else if (data.compare(i, 6, "&apos;") == 0)
            {
                unescaped.push_back('\'');
                i += 5;
            }
            else
            {
                // entità sconosciuta: copia & così com'è
                unescaped.push_back('&');
            }
        }
        else
        {
            unescaped.push_back(data[i]);
        }
    }
    return unescaped;
}

void saveConcertsToXML(const std::vector<Concert> &concerts)
{
    std::string path = std::string(std::getenv("HOME")) + "/concerti.xml";
    std::ofstream file(path);
    if (!file)
        return;

    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<concerts>\n";

    for (const auto &concert : concerts)
    {
        file << "  <concert>\n";
        file << "    <title>" << concert.getTitle() << "</title>\n";
        file << "    <comment>" << xmlEscape(concert.getComment()) << "</comment>\n";

        file << "    <places>\n";
        for (const auto &place : concert.getPlaces())
            file << "      <place>" << place << "</place>\n";
        file << "    </places>\n";

        file << "    <dates>\n";
        for (const auto &date : concert.getDates())
            file << "      <date>" << date << "</date>\n";
        file << "    </dates>\n";

        file << "    <musicians>\n";
        for (const auto &m : concert.getMusicians())
        {
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
        for (const auto &p : concert.getProgram())
        {
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

std::string getTagValue(const std::string &line, const std::string &tag)
{
    auto start = line.find("<" + tag + ">");
    auto end = line.find("</" + tag + ">");
    if (start == std::string::npos || end == std::string::npos)
        return "";
    start += tag.length() + 2;
    return line.substr(start, end - start);
}

std::vector<Concert> loadConcertsFromXML()
{
    LOG_MSG("Loading concerts from XML");
    std::vector<Concert> concerts;
    std::ifstream file(std::string(std::getenv("HOME")) + "/concerti.xml");
    if (!file)
    {
        LOG_MSG("Could not open file");
        return concerts;
    }

    std::string line;
    Concert currentConcert;
    std::vector<std::string> places, dates;
    std::vector<Musician> musicians;
    std::vector<MusicalPiece> program;
    std::string comment;

    while (std::getline(file, line))
    {
        if (line.find("<concert>") != std::string::npos)
        {
            currentConcert = Concert();
            places.clear();
            dates.clear();
            musicians.clear();
            program.clear();
            comment.clear();
        }
        else if (line.find("<title>") != std::string::npos)
        {
            currentConcert.setTitle(getTagValue(line, "title"));
        }
        else if (line.find("<place>") != std::string::npos)
        {
            places.push_back(getTagValue(line, "place"));
        }
        else if (line.find("<date>") != std::string::npos)
        {
            dates.push_back(getTagValue(line, "date"));
        }
        else if (line.find("<comment>") != std::string::npos)
        {
            std::string rawComment = getTagValue(line, "comment");
            currentConcert.setComment(xmlUnescape(rawComment));
        }
        else if (line.find("<musician>") != std::string::npos)
        {
            Musician m;
            if (!std::getline(file, line))
                break;
            m.setName(getTagValue(line, "name"));

            if (!std::getline(file, line))
                break;
            m.setPhone(getTagValue(line, "phone"));

            if (!std::getline(file, line))
                break;
            m.setInstrument(getTagValue(line, "instrument"));

            if (!std::getline(file, line))
                break;
            m.setMail(getTagValue(line, "email"));

            if (!std::getline(file, line))
                break;
            m.setAddress(getTagValue(line, "address"));

            if (!std::getline(file, line))
                break;
            try
            {
                m.setGage(std::stod(getTagValue(line, "gage")));
            }
            catch (...)
            {
                m.setGage(0.0);
                LOG_MSG("Invalid gage value, set to 0");
            }

            musicians.push_back(m);
        }
        else if (line.find("<piece>") != std::string::npos)
        {
            MusicalPiece p("", "", "", "", false, "", "");

            if (!std::getline(file, line))
                break;
            p.setComposer(getTagValue(line, "composer"));

            if (!std::getline(file, line))
                break;
            p.setTitle(getTagValue(line, "title"));

            if (!std::getline(file, line))
                break;
            p.setKey(getTagValue(line, "key"));

            if (!std::getline(file, line))
                break;
            p.setDuration(getTagValue(line, "duration"));

            if (!std::getline(file, line))
                break;
            p.setChoir(getTagValue(line, "choir") == "1");

            if (!std::getline(file, line))
                break;
            p.setSingerPart(getTagValue(line, "singerPart"));

            if (!std::getline(file, line))
                break;
            p.setInstruments(getTagValue(line, "instruments"));

            program.push_back(p);
        }
        else if (line.find("</concert>") != std::string::npos)
        {
            currentConcert.setPlaces(places);
            currentConcert.setDates(dates);
            currentConcert.setMusicians(musicians);
            currentConcert.setProgram(program);
            concerts.push_back(currentConcert);
        }
    }

    LOG_MSG("Finished loading concerts");
    return concerts;
}

int main()
{
    std::vector<Concert> concerts = loadConcertsFromXML(); // carica
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
        case MainMenuView::ADD_CONCERT:
        {
            auto concert = controller.createConcert();
            if (concert)
                concerts.push_back(*concert);
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
            saveConcertsToXML(concerts); // salva prima di uscire
            endwin();
            return 0;
        }
    }
}
