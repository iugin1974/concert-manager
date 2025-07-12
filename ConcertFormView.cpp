#include "ConcertFormView.h"
#include "musician_form.h"
#include "rehearsal_form.h"
#include "pieceForm.h"
#include "PopupMenu.h"
#include "logMessage.h"
#include "Utils.h"
#include <form.h>
#include <ncurses.h>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <algorithm>
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

bool ConcertFormView::editComment(std::string &comment)
{
  const char *tmpFile = "/tmp/concert_comment.txt";

  // 1) Scrivi il commento corrente sul file temporaneo
  {
    std::ofstream out(tmpFile);
    if (!out)
      return false;
    out << comment;
  }

  // 2) Fork + exec dell’editor
  pid_t pid = fork();
  if (pid == 0)
  {
    execlp("vim", "vim", tmpFile, (char *)nullptr);
    _exit(1); // se execlp fallisce
  }
  else if (pid > 0)
  {
    int status;
    if (waitpid(pid, &status, 0) == -1)
      return false;
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
      return false;

    // 3) Rileggi direttamente su 'comment'
    std::ifstream in(tmpFile);
    if (!in)
      return false;
    comment.clear();
    std::string line;
    while (std::getline(in, line))
    {
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
bool ConcertFormView::show(const Concert *existing)
{

  if (existing) {
    title = existing->getTitle();
    places = existing->getPlaces();
    dates = existing->getDatesAsString(); 
    musicians = existing->getMusicians();
    program = existing->getProgram();
    comment = existing->getComment();
    rehearsals = existing->getRehearsals();
  }

  const int NUM_FIELDS = 3;

redraw:
  clear();
  FormComponents fc = drawForm(existing);
  FIELD **fields = fc.fields;
  FORM *form = fc.form;

  set_field_buffer(fields[0], 0, title.c_str());
  set_field_buffer(fields[1], 0, join(places, ", ").c_str());
  set_field_buffer(fields[2], 0, join(dates, ", ").c_str()); 

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
	    "Save",
	    "Exit without saving",
	    "Add Musician",
	    "Edit Musician",
	    "Delete Musician",
	    "Add Piece",
	    "Edit Piece",
	    "Delete Piece",
	    "Add Rehearsal",
	    "Edit Rehearsal",
	    "Delete Rehearsal",
	    "Comment",
	    "TODO"};
	  PopupMenu popup(stdscr, actions);
	  int selected = popup.show();

	  form_driver(form, REQ_VALIDATION);
	  title = trim(field_buffer(fields[0], 0));
	  places = split(field_buffer(fields[1], 0));
	  dates = split(field_buffer(fields[2], 0));

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
	    case 1: // Save

	      break;
	    case 2: // Exit without saving
	      title.clear();
	      places.clear();
	      dates.clear();
	      done = true;
	      clear();
	      refresh();
	      return false;

	    case 3:
	      { // Add musician
		Musician m;
		if (runMusicianForm(nullptr, m))
		  musicians.push_back(m);
		break;
	      }

	    case 4:
	      { // Edit musician
		int idx = runChoiceForm(musicians);
		if (idx == -1) break;
		Musician m;
		bool changed = runMusicianForm(&musicians.at(idx), m);
		if (changed) {
		  musicians.erase(musicians.begin() + idx);
		  musicians.push_back(m);
		}
		break;
	      }

	    case 5:

	      { // Delete musician
		int idx = runChoiceForm(musicians);
		if (idx == -1) break;
		bool del = confirmDialog(stdscr);
		if (del) {
		  musicians.erase(musicians.begin() + idx);
		}
		break;
	      }

	    case 6:
	      { // add piece
		MusicalPiece newPiece("", "", 0, false, "", "");
		if (runPieceForm(nullptr, newPiece))
		{
		  program.push_back(newPiece); // aggiungi al vettore
		}
		break;
	      }

	    case 7:
	      { // edit piece
		int choice = runChoiceForm(program);
		if (choice == -1) break;
		MusicalPiece newPiece("", "", 0, false, "", "");
		bool added = runPieceForm(&program.at(choice), newPiece);
		if (added) {
		  program.erase(program.begin() + choice);
		  program.push_back(newPiece);
		}
		break;
	      }

	    case 8:
	      { //  delete piece
		int idx = runChoiceForm(program);
		if (idx == -1) break;
		bool del = confirmDialog(stdscr);
		if (del) {
		  program.erase(program.begin() + idx);
		}
		break;
	      }
	    case 9: { // add rehearsal
Rehearsal newRehearsal;
if (runRehearsalForm(nullptr, newRehearsal)) {
rehearsals.push_back(newRehearsal);
    }
		      break;
		    }
	    case 10: { // edit rehearsal
	int choice = runChoiceForm(rehearsals);
		if (choice == -1) break;
		Rehearsal newRehearsal;
		bool added = runRehearsalForm(&rehearsals.at(choice), newRehearsal);
		if (added) {
		  rehearsals.erase(rehearsals.begin() + choice);
		  rehearsals.push_back(newRehearsal);
		}
		       break;
		     }
	    case 11: { // delete rehearsal
int idx = runChoiceForm(rehearsals);
		if (idx == -1) break;
		bool del = confirmDialog(stdscr);
		if (del) {
		  rehearsals.erase(rehearsals.begin() + idx);
		}
		       break;
		     }
	    case 12:
		     { // Comment
		       editComment(comment);
		       // TODO verifica se ritorna true o false
		       break;
		     }

	    case 13:
		     { // todo
		       form_driver(form, REQ_VALIDATION);
		       const char *raw = field_buffer(fields[0], 0);
		       todo_filename = raw;

		       // Rimuove spazi finali
		       todo_filename.erase(std::find_if(todo_filename.rbegin(), todo_filename.rend(), [](char c)
			     { return !std::isspace(static_cast<unsigned char>(c)); })
			   .base(),
			   todo_filename.end());
		       todo_filename += ".xml";

		       pid_t pid = fork();

		       if (pid == 0)
		       {
			 // Processo figlio: esegue il programma

			 execlp("pm", "pm", "--file", todo_filename.c_str(), nullptr);
			 // Se execlp fallisce
			 perror("execlp");
			 return 1;
		       }
		       else if (pid > 0)
		       {
			 // Processo padre: attende il completamento
			 int status;
			 waitpid(pid, &status, 0);
		       }
		       else
		       {
			 perror("fork");
			 return 1;
		       }

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
std::vector<MusicalPiece> ConcertFormView::getProgram() const { return program; }
std::vector<Rehearsal> ConcertFormView::getRehearsals() const { return rehearsals; }
std::string ConcertFormView::getComment() const { return comment; }
std::string ConcertFormView::getToDo() const { return todo_filename; }
