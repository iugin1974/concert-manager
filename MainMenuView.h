#pragma once

class MainMenuView {
public:
    enum Action {
        ADD_CONCERT,
        MANAGE_CONCERTS,
        EXIT
    };

    Action show();  // mostra il menu, ritorna l’azione scelta
};

