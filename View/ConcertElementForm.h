#pragma once
#include "View.h"
#include <form.h>
#include <ncurses.h>

// Base comune per le View basate su <form.h> che mostrano/editano
// un singolo elemento (Musician, MusicalPiece, Rehearsal).
//
// T = tipo del modello (Musician, MusicalPiece, Rehearsal)
// N = dimensione dell'array fields, INCLUSO il terminatore nullptr finale
//     (es. 13 per Musician = 12 campi dati + 1 nullptr)
template <typename T, int N>
class ConcertElementForm : public View {
public:
    static constexpr int NUMBER_OF_FIELDS = N;

    ~ConcertElementForm() override {
        closeForm();
    }

    void setElement(T *e) { element = e; }

    // ---- Comportamento comune ----

    void closeForm() override {
        if (!form) return;
        unpost_form(form);
        free_form(form);
        for (int i = 0; fields[i] != nullptr; ++i) {
            free_field(fields[i]);
        }
        form = nullptr;
    }

    void clearFormFields() override {
        for (int i = 0; i < N - 1; ++i) {
            set_field_buffer(fields[i], 0, "");
        }
        onClearExtra();          // hook: reset di stato extra (es. checkbox)
        if (form) form_driver(form, REQ_FIRST_FIELD);
        refresh();
    }

    void show() override {
        if (!form) init_form();
        post_form(form);
        printLabels();           // hook: stampa le label specifiche
        set_current_field(form, fields[0]);
        form_driver(form, REQ_FIRST_FIELD);
        form_driver(form, REQ_END_LINE);
        refresh();
    }

    MenuCommand getCommand() override {
        int ch;
        FIELD *prevField = current_field(form);
        while (true) {
            ch = getch();

            if (!handleSpecialKey(ch)) {   // hook: tasti specifici (es. spazio checkbox)
                switch (ch) {
                case 10:        // Enter
                case KEY_DOWN:
                case 9:         // TAB
                    form_driver(form, REQ_NEXT_FIELD);
                    form_driver(form, REQ_END_LINE);
                    break;
                case KEY_UP:
                case KEY_BTAB:
                    form_driver(form, REQ_PREV_FIELD);
                    form_driver(form, REQ_END_LINE);
                    break;
                case KEY_BACKSPACE:
                case 127:
                case '\b':
                    form_driver(form, REQ_DEL_PREV);
                    break;
                case KEY_DC:
                    form_driver(form, REQ_DEL_CHAR);
                    break;
                case KEY_F(2): {
                    MenuCommand result = menuBar.show();
                    if (shouldSaveOnCommand(result)) {
                        saveDataFromForm();
                    }
                    return result;
                }
                default:
                    form_driver(form, ch);
                    break;
                }
            }

            FIELD *currField = current_field(form);
            if (currField != prevField) {
                onFieldChanged();   // hook: reazione al cambio campo (es. autofill)
                prevField = currField;
            }
            refresh();
        }
    }

protected:
    T *element = nullptr;
    FIELD *fields[N] = {};
    FORM *form = nullptr;

    // ---- Hook per le sottoclassi ----
    virtual void printLabels() = 0;
    virtual bool handleSpecialKey(int /*ch*/) { return false; }
    virtual void onFieldChanged() {}
    virtual void onClearExtra() {}
    // Default: salva su qualsiasi comando diverso da Quit.
    // Override nelle sottoclassi che hanno comandi extra "non di salvataggio"
    // (es. PieceForm con AddScore/DeleteScore).
    virtual bool shouldSaveOnCommand(MenuCommand cmd) const {
        return cmd != MenuCommand::Quit;
    }

    // ---- Resta specifico di ciascuna sottoclasse ----
    void init_form() override = 0;
    void saveDataFromForm() override = 0;
    void updateFields() override = 0;
};
