#include "MainMenuView.h"
#include <ncurses.h>

MainMenuView::Action MainMenuView::show()
{
   
    const char *choices[] = {
        "1. Add Concert",
        "2. View Concerts",
        "3. Edit Concert",
        "4. Delete Concert",
        "5. Exit",
    };

    int n_choices = sizeof(choices) / sizeof(choices[0]);

    for (int i = 0; i < n_choices; ++i)
    {
        mvprintw(i + 1, 2, choices[i]);
    }

    while (1)
    {
        int c = getch();
        c = c - 49;
        if (c < 0 || c > 5 + n_choices)
            continue;
        return static_cast<Action>(c);
    }
}
