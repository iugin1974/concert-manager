#include "ConcertFormView.h"
#include "musician_form.h"
#include "pieceForm.h"
#include "PopupMenu.h"
#include "logMessage.h"
#include <form.h>
#include <ncurses.h>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <sys/wait.h>
// Costruttori
ConcertFormView::ConcertFormView() {}

ConcertFormView::ConcertFormView(const Concert &c)
{
    title = c.getTitle();
    dates = c.getDatesAsString();
    places = c.getPlaces();
    musicians = c.getMusicians();
	comment = c.getComment();
}

// Helper: trim
static std::string trim(const char *buffer)
{
    std::string s(buffer);
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

// Helper: split da stringa separata da virgole
static std::vector<std::string> split(const std::string &input, char sep = ',')
{
    std::stringstream ss(input);
    std::string item;
    std::vector<std::string> result;
    while (std::getline(ss, item, sep))
    {
        result.push_back(trim(item.c_str()));
    }
    return result;
}

bool ConcertFormView::editComment(std::string& comment) {
    const char* tmpFile = "/tmp/concert_comment.txt";

    // 1) Scrivi il commento corrente sul file temporaneo
    {
        std::ofstream out(tmpFile);
        if (!out) return false;
        out << comment;
    }

    // 2) Fork + exec dell’editor
    pid_t pid = fork();
    if (pid == 0) {
        execlp("vim", "vim", tmpFile, (char*)nullptr);
        _exit(1);  // se execlp fallisce
    }
    else if (pid > 0) {
        int status;
        if (waitpid(pid, &status, 0) == -1) return false;
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) return false;

        // 3) Rileggi direttamente su 'comment'
        std::ifstream in(tmpFile);
        if (!in) return false;
        comment.clear();
        std::string line;
        while (std::getline(in, line)) {
            comment += line;
            comment += '\n';
        }
        // Rimuovi l’ultimo '\n' in eccesso
        if (!comment.empty() && comment.back() == '\n')
            comment.pop_back();

        // 4) Pulisci il file temporaneo
        std::remove(tmpFile);
        return true;
    }

    // fork fallito
    return false;
}


struct FormComponents
{
    FIELD *fields[4]; // 3 input + nullptr
    FORM *form;
};

// Disegna il form
FormComponents drawForm(const Concert *existing)
{
    FormComponents fc;

    fc.fields[0] = new_field(1, 40, 2, 30, 0, 0); // Title
    fc.fields[1] = new_field(1, 40, 4, 30, 0, 0); // Places
    fc.fields[2] = new_field(1, 40, 6, 30, 0, 0); // Dates
    fc.fields[3] = nullptr;

    for (int i = 0; i < 3; ++i)
    {
        set_field_back(fc.fields[i], A_UNDERLINE);
        field_opts_off(fc.fields[i], O_AUTOSKIP);
    }

    if (existing)
    {
        set_field_buffer(fc.fields[0], 0, existing->getTitle().c_str());

        std::string placesStr, datesStr;
        for (size_t i = 0; i < existing->getPlaces().size(); ++i)
        {
            if (i > 0)
                placesStr += ", ";
            placesStr += existing->getPlaces()[i];
        }
        for (size_t i = 0; i < existing->getDatesAsString().size(); ++i)
        {
            if (i > 0)
                datesStr += ", ";
            datesStr += existing->getDatesAsString()[i];
        }

        set_field_buffer(fc.fields[1], 0, placesStr.c_str());
        set_field_buffer(fc.fields[2], 0, datesStr.c_str());
    }

    fc.form = new_form(fc.fields);
    post_form(fc.form);

    mvprintw(2, 5, "Title:");
    mvprintw(4, 5, "Places (comma-separated):");
    mvprintw(6, 5, "Dates  (comma-separated):");
    mvprintw(8, 5, "F2 = Actions");

    refresh();
    form_driver(fc.form, REQ_FIRST_FIELD);
    return fc;
}

// Metodo show
bool ConcertFormView::show(const Concert* existing)
{
    const int NUM_FIELDS = 3;

    std::string tempTitle = title;
    std::string tempPlaces, tempDates;

    for (size_t i = 0; i < places.size(); ++i)
    {
        if (i > 0)
            tempPlaces += ", ";
        tempPlaces += places[i];
    }
    for (size_t i = 0; i < dates.size(); ++i)
    {
        if (i > 0)
            tempDates += ", ";
        tempDates += dates[i];
    }

redraw:
    clear();
    FormComponents fc = drawForm(existing);
    FIELD **fields = fc.fields;
    FORM *form = fc.form;

    set_field_buffer(fields[0], 0, tempTitle.c_str());
    set_field_buffer(fields[1], 0, tempPlaces.c_str());
    set_field_buffer(fields[2], 0, tempDates.c_str());

    bool done = false;
    int ch;

    while (!done)
    {
        FIELD *current = current_field(form);
        int currentIndex = field_index(current);

        ch = getch();
        switch (ch)
        {
        case 10: // ENTER
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            break;

        case KEY_DOWN:
        case 9: // TAB
            form_driver(form, REQ_NEXT_FIELD);
            break;

        case KEY_UP:
            form_driver(form, REQ_PREV_FIELD);
            break;

        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;

        case KEY_F(2):
        { // MENU AZIONI
            std::vector<std::string> actions = {
                "Save and Exit",
                "Exit without saving",
                "Add Musician",
                "Edit Musician",
                "Delete Musician",
                "Add Piece",
                "Edit Piece",
                "Delete Piece",
				"Comment"};
            PopupMenu popup(stdscr, actions);
            int selected = popup.show();

            form_driver(form, REQ_VALIDATION);
            tempTitle = trim(field_buffer(fields[0], 0));
            tempPlaces = trim(field_buffer(fields[1], 0));
            tempDates = trim(field_buffer(fields[2], 0));

            switch (selected)
            {
            case 0: // Save and Exit
                form_driver(form, REQ_VALIDATION);
                title = trim(field_buffer(fields[0], 0));
                places = split(field_buffer(fields[1], 0));
                dates = split(field_buffer(fields[2], 0));
                done = true;
                clear();
                refresh();
                return true;

            case 1: // Exit without saving
                title.clear();
                places.clear();
                dates.clear();
                done = true;
                clear();
                refresh();
                return false;

            case 2:
            { // Add musician
                Musician m;
                if (runMusicianForm(nullptr, m))
                    musicians.push_back(m);
                break;
            }

            case 3:
            { // Edit musician
                int idx = runChoiceMusicianForm(musicians);
                if (idx == -1)
                    return false;
                // TODO
                break;
            }

            case 4:
            { // Delete musician
                int idx = runChoiceMusicianForm(musicians);
                if (idx >= 0 && idx < (int)musicians.size())
                    musicians.erase(musicians.begin() + idx);
                break;
            }

            case 5:
            { // add piece
                MusicalPiece newPiece("", "", "", "", false, "", "");
                if (runPieceForm(nullptr, newPiece))
                {
                    pieces.push_back(newPiece); // aggiungi al vettore
                }
                break;
            }

            case 6:
            { // edit piece
                break;
            }

            case 7:
            { // delete piece
                break;
            }

			case 8: 
			{ // Comment
			editComment(comment);
			// TODO verifica se ritorna true o false
			break;
			}
            }

            unpost_form(form);
            free_form(form);
            for (int i = 0; i < NUM_FIELDS + 1; ++i)
                free_field(fields[i]);
            goto redraw;
        }

        default:
            if (currentIndex < NUM_FIELDS)
                form_driver(form, ch);
            break;
        }
    }

    // Pulizia finale
    unpost_form(form);
    free_form(form);
    for (int i = 0; i < NUM_FIELDS + 1; ++i)
        free_field(fields[i]);
return true;
}

// Getters
std::string ConcertFormView::getTitle() const { return title; }
std::vector<std::string> ConcertFormView::getPlaces() const { return places; }
std::vector<std::string> ConcertFormView::getDates() const { return dates; }
std::vector<Musician> ConcertFormView::getMusicians() const { return musicians; }
std::vector<MusicalPiece> ConcertFormView::getProgram() const { LOG_MSG ("Mando pieces"); return pieces; }
std::string ConcertFormView::getComment() const { return comment; }
