#include "ConcertController.h"
#include "ConcertFormView.h"
#include "ConcertSummaryView.h"
#include "NcursesMenuBar.h"
#include "logMessage.h"
#include "Utils.h"
#include <algorithm>  // std::sort
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

  while (true)
{
    mvprintw(0, 0, "Select any concert [ESC to return]:");
    for (size_t i = 0; i < concerts.size(); ++i)
    {
        mvprintw(i + 2, 2, "%zu. %s", i + 1, concerts[i].getTitle().c_str());
    }

    int ch = getch();

    if (ch == 27) // ESC
        return -1;

    // Controlla se è una cifra valida '1' .. 'n'
    if (ch >= '1' && ch <= '0' + static_cast<int>(concerts.size()))
    {
        int index = ch - '1'; // Converte '1' in 0, '2' in 1, ecc.
        return index;
    }
}
 }

void ConcertController::listConcerts(std::vector<Concert>& concerts) {
  while (true) {
    int choice = selectConcert(concerts);
    if (choice == -1) return;

    Concert& selected = concerts[choice];

    while (true) {
      // 1. Mostra il resoconto
      clear();
      ConcertSummaryView view;
      view.show(selected);

      // 2. Menu azioni
      std::vector<std::string> menuTitles = {"Actions"};
      std::vector<std::vector<std::string>> menuItems = {
          {"Edit", "Delete", "Back"}
      };

      NcursesMenuBar bar(menuTitles, menuItems);
      std::pair<int, int> result = bar.activate();

      int action = result.second;

      if (action == 0) { // Edit
        std::optional<Concert> updated = editConcertSingle(selected);
        if (updated)
          selected = *updated;
      } else if (action == 1) { // Delete
        if (confirmDialog(stdscr)) {
          concerts.erase(concerts.begin() + choice);
          break; // torna alla lista aggiornata
        }
      } else if (action == 2 || action == -1) { // Back o ESC
        break;
      }
    }
  }
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
  concert.setToDo(view.getToDo());
  concert.setPlaces(view.getPlaces());
  concert.setDatesAsString(view.getDates());
  concert.setMusicians(view.getMusicians());
  concert.setProgram(view.getProgram());
  concert.setRehearsals(view.getRehearsals());
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
  updated.setToDo(view.getToDo());
  updated.setDatesAsString(view.getDates());
  updated.setProgram(existing.getProgram());
  updated.setMusicians(view.getMusicians());
updated.setRehearsals(view.getRehearsals());
  updated.setProgram(view.getProgram());
  updated.setComment(view.getComment());
  LOG_MSG("Concert updated");
  return updated;
}
