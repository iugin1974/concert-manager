#include "MainMenuView.h"
#include <ncurses.h>

MainMenuView::Action MainMenuView::show()
{
  attron(A_BOLD);
  mvprintw (1, 2, "Concert Manager - by Eugenio Giovine");
  mvprintw (3, 2, "Menu:");
  attroff(A_BOLD);
  const char *choices[] = {
    "1. Add Concert",
    "2. Manage Concerts",
    "3. Save",
    "4. Save and exit"
  };

  int n_choices = sizeof(choices) / sizeof(choices[0]);

  for (int i = 0; i < n_choices; ++i)
  {
    mvprintw(i + 5, 2, "%s", choices[i]);
  }

  while (1)
  {
    int c = getch();
    c = c - 49;
    if (c < 0 || c > 3 + n_choices)
      continue;
    return static_cast<Action>(c);
  }
}
