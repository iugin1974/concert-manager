#include "musician_form.h"
#include "PopupMenu.h"
#include "logMessage.h"
#include "Utils.h"
#include <form.h>
#include <string>
#include <locale.h>

int runChoiceMusicianForm(std::vector<Musician>& musicians) {
    clear();
    refresh();

    int l = musicians.size();
    if (l == 0) {
        mvprintw(0, 0, "No musicians in Concert");
        refresh();
        getch();
        return -1;
    }

    // Mostra lista
    for (int i = 0; i < l; i++) {
        mvprintw(i, 3, "%d. %s", i + 1, musicians.at(i).getName().c_str()); // numeri da 1
    }

    mvprintw(l + 2, 3, "Quale musicista scegli?");

    // Leggi scelta con promptNumber
    int choice = promptNumber(stdscr, l + 3, 3, 1, l);
	if (choice == 0) return -1;
    if (choice < 1 || choice > l) {
        mvprintw(l + 4, 3, "Scelta non valida");
        refresh();
        getch();
        return -1;
    }

    return choice - 1;  // ritorna indice cancellato
}


bool runMusicianForm(const Musician* existing, Musician& musician) {
    FIELD* fields[7];  // 6 campi + nullptr
    FORM* form;

    // Crea i campi (posizionati su righe diverse)
    fields[0] = new_field(1, 40, 2, 20, 0, 0); // Name
    fields[1] = new_field(1, 40, 4, 20, 0, 0); // Phone
    fields[2] = new_field(1, 40, 6, 20, 0, 0); // Instrument
    fields[3] = new_field(1, 40, 8, 20, 0, 0); // Mail
    fields[4] = new_field(1, 40,10, 20, 0, 0); // Address
    fields[5] = new_field(1, 40,12, 20, 0, 0); // Gage
    fields[6] = nullptr;

    for (int i = 0; i < 6; ++i) {
        set_field_back(fields[i], A_UNDERLINE);   // Campo visibile
        field_opts_off(fields[i], O_AUTOSKIP);    // Non salta automaticamente
    }

    // Precompilazione se "existing" presente
    if (existing) {
        set_field_buffer(fields[0], 0, existing->getName().c_str());
        set_field_buffer(fields[1], 0, existing->getPhone().c_str());
        set_field_buffer(fields[2], 0, existing->getInstrument().c_str());
        set_field_buffer(fields[3], 0, existing->getMail().c_str());
        set_field_buffer(fields[4], 0, existing->getAddress().c_str());
        set_field_buffer(fields[5], 0, std::to_string(existing->getGage()).c_str());
    }

    // Crea e configura il form
    form = new_form(fields);
    post_form(form);
    refresh(); // stdscr

    // Etichette
    mvprintw(2,  2, "Name:");
    mvprintw(4,  2, "Phone:");
    mvprintw(6,  2, "Instrument:");
    mvprintw(8,  2, "Mail:");
    mvprintw(10, 2, "Address:");
    mvprintw(12, 2, "Gage:");
    mvprintw(14, 2, "Press ENTER to confirm");

    refresh();

    form_driver(form, REQ_FIRST_FIELD);

    int ch;
    while (true) {
ch = getch();
        switch (ch) {
			case KEY_F(2): { // 🔧 MENU AZIONI
						    std::vector<std::string> actions = {
				"Save and Exit",
				"Exit without saving"
			};
                PopupMenu popup(stdscr, actions, 10, 12);
                int selected = popup.show();
					switch (selected) {
						case 0: { // save and exit
								form_driver(form, REQ_VALIDATION);

								// Estrai i dati
								auto trim = [](std::string s) {
									s.erase(s.find_last_not_of(" \n") + 1);
									s.erase(0, s.find_first_not_of(" "));
									return s;
								};

								std::string name     = trim(field_buffer(fields[0], 0));
								std::string phone    = trim(field_buffer(fields[1], 0));
								std::string instr    = trim(field_buffer(fields[2], 0));
								std::string mail     = trim(field_buffer(fields[3], 0));
								std::string address  = trim(field_buffer(fields[4], 0));
								std::string gage_str = trim(field_buffer(fields[5], 0));
								double gage = std::stod(gage_str.empty() ? "0" : gage_str);

								// Se vuoto, non salvare
								if (name.empty()) return false;

								// Imposta il musicista
								musician.setName(name);
								musician.setPhone(phone);
								musician.setInstrument(instr);
								musician.setMail(mail);
								musician.setAddress(address);
								musician.setGage(gage);

								// Pulizia
								unpost_form(form);
								free_form(form);
								for (int i = 0; i < 6; ++i)
									free_field(fields[i]);

								return true;
								}
case 1: // exit without saving
return false;
}
break;
}

			case 10: // Enter
            case KEY_DOWN:
            case 9: // TAB
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
            default:
                form_driver(form, ch);
                break;
        }
        refresh();  // Aggiorna stdscr dopo ogni input
    }

    
}
