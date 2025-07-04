#include "ConcertController.h"
#include "ConcertFormView.h"
#include "ConcertSummaryView.h"
#include "logMessage.h"
#include <ncurses.h>
#include <optional>
int ConcertController::selectConcert(std::vector<Concert> &concerts)
{
    clear();
    mvprintw(0, 0, "List of Concerts:");
    if (concerts.empty())
    {
        mvprintw(0, 0, "No concerts in database. Press any key.");
        refresh();
        getch();
        return -1;
    }

    int choice = 0;
    while (true)
    {
        mvprintw(0, 0, "Select any concert [ESC to return]:");
        for (size_t i = 0; i < concerts.size(); ++i)
        {
            if ((int)i == choice)
                attron(A_REVERSE);
            mvprintw(i + 2, 2, "%zu. %s", i + 1, concerts[i].getTitle().c_str());
            if ((int)i == choice)
                attroff(A_REVERSE);
        }

        int ch = getch();
        if (ch == '\n')
            return choice;
        if (ch == 27)
            return -1;
        if (ch == KEY_UP)
            choice = (choice - 1 + concerts.size()) % concerts.size();
        if (ch == KEY_DOWN)
            choice = (choice + 1) % concerts.size();
    }
}

void ConcertController::listConcerts(std::vector<Concert> &concerts)
{
    int choice = selectConcert(concerts);
    if (choice == -1)
    {
        LOG_MSG("Choice -1 - exit");
        return;
    }
    ConcertSummaryView summaryView;
    Concert concert = concerts.at(choice);
    summaryView.show(concert);
    clear();
    refresh();
}

std::optional<Concert> ConcertController::createConcert()
{
    ConcertFormView view;
    bool save = view.show();
    if (!save)
    {
        LOG_MSG("Concert not saved");
        return std::nullopt;
    }

    Concert concert(view.getTitle());
    concert.setPlaces(view.getPlaces());
    concert.setDates(view.getDates());
    concert.setMusicians(view.getMusicians());
    concert.setProgram(view.getProgram());
    concert.setComment(view.getComment());
    LOG_MSG("Return concert");
    return concert;
}

void ConcertController::editConcert(std::vector<Concert> &concerts)
{
    clear();
    if (concerts.empty())
    {
        mvprintw(0, 0, "No concerts to edit. Press any key.");
        refresh();
        getch();
        return;
    }

    int choice = selectConcert(concerts);
    if (choice == -1)
    {
        LOG_MSG("Choice -1");
        return;
    }
    std::optional<Concert> concert = editConcertSingle(concerts[choice]);
    if (concert)
        concerts[choice] = *concert;
    LOG_MSG("concert edited - Return");
}

void ConcertController::deleteConcert(std::vector<Concert> &concerts)
{

    clear();
    if (concerts.empty())
    {
        mvprintw(0, 0, "No concerts to delete. Press any key.");
        refresh();
        getch();
        return;
    }
    int choice = selectConcert(concerts);
    if (choice == -1)
    {
        LOG_MSG("Choice -1");
        return;
    }
    concerts.erase(concerts.begin() + choice);
    LOG_MSG("Concert erased");
}

std::optional<Concert> ConcertController::editConcertSingle(const Concert &existing)
{
    ConcertFormView view(existing);
    int saved = view.show(&existing);
    if (!saved)
    {
        LOG_MSG("Concert not saved");
        return std::nullopt;
    }

    Concert updated(view.getTitle());
    updated.setPlaces(view.getPlaces());
    updated.setDates(view.getDates());
    updated.setProgram(existing.getProgram()); // oppure aggiorna anche questo se la form è completa
    updated.setMusicians(view.getMusicians()); // stesso discorso
    updated.setProgram(view.getProgram());
    updated.setComment(view.getComment());
    LOG_MSG("Concert updated");
    return updated;
}
