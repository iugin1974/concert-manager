#pragma once

class MainMenuView {
public:
    enum Action {
        ADD_CONCERT,
        VIEW_CONCERTS,
        EDIT_CONCERT,
        DELETE_CONCERT,
        EXIT
    };

    Action show();  // mostra il menu, ritorna l’azione scelta
};

