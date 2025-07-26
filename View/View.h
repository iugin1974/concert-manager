#pragma once
#include "MenuBar.h"

class View {
public:
    View() = default;
    virtual ~View() = default;

    // Imposta la barra del menu da visualizzare (implementazione comune)
    virtual void setMenuBar(const MenuBar& bar) {
        menuBar = bar;
    }

    // Esegue la visualizzazione e ritorna il comando scelto dall'utente
    virtual void show() = 0;
    virtual MenuCommand getCommand() = 0;
    virtual void validateFields() = 0;

protected:
    MenuBar menuBar; // comune a tutte le view
    virtual void init_form() = 0;
    virtual void closeForm() = 0;
};
