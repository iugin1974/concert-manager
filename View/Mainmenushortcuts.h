#pragma once
#include <map>
#include <utility>
#include <optional>
#include <ncurses.h>
#include "MenuBar.h"

// Mappa delle shortcut del menu principale: {primo tasto, secondo tasto} -> comando.
// Aggiungere una nuova shortcut = aggiungere una riga qui, niente altro.
inline const std::map<std::pair<char, char>, MenuCommand> mainMenuShortcuts = {
    { {'a', 'm'}, MenuCommand::AddMusician },
    { {'a', 'p'}, MenuCommand::AddPiece },
    { {'a', 'r'}, MenuCommand::AddRehearsal },
    { {'d', 'm'}, MenuCommand::DeleteMusician },
    { {'d', 'p'}, MenuCommand::DeletePiece },
    { {'d', 'r'}, MenuCommand::DeleteRehearsal },
    { {'e', 'm'}, MenuCommand::EditMusician },
    { {'e', 'p'}, MenuCommand::EditPiece },
    { {'e', 'r'}, MenuCommand::EditRehearsal },
};

// Da chiamare quando il primo tasto letto nel loop principale è 'a', 'd' o 'e'
// (cioè potrebbe essere l'inizio di una shortcut).
// Mostra un prompt, legge il secondo tasto, e ritorna il comando se la
// combinazione esiste. Ritorna std::nullopt se non valida o se l'utente
// annulla con ESC.
inline std::optional<MenuCommand> readShortcutSecondKey(char firstKey, int promptRow, int promptCol) {
    // Feedback visivo: mostra il primo tasto e aspetta il secondo
    mvprintw(promptRow, promptCol, "%c_", firstKey);
    refresh();

    int ch = getch();

    // Pulisce il prompt indipendentemente dall'esito
    mvprintw(promptRow, promptCol, "  ");
    refresh();

    if (ch == 27 /* ESC */) {
        return std::nullopt;
    }

    auto it = mainMenuShortcuts.find({ firstKey, static_cast<char>(ch) });
    if (it == mainMenuShortcuts.end()) {
        beep();  // feedback di combinazione non valida
        return std::nullopt;
    }

    return it->second;
}

// Da chiamare nel loop principale per il primo tasto.
// Ritorna true se 'ch' potrebbe essere l'inizio di una shortcut (a/d/e),
// nel qual caso il chiamante deve poi invocare readShortcutSecondKey().
inline bool isShortcutPrefix(int ch) {
    return ch == 'a' || ch == 'd' || ch == 'e';
}
