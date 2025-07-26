# Concert Manager

Concert Manager is a terminal-based application (TUI) to manage concerts, musical pieces, rehearsals, musicians, and associated scores. It follows the MVC (Model-View-Controller) architecture and is written in modern C++.

---

## Project Structure

```
├── Controller/         # Controllers handling logic and user actions
│   └── ConcertController.{h,cpp}
│
├── Core/               # Core utilities and general-purpose code
│   ├── logMessage.{h,cpp}       # Logging utility
│   └── Utils.{h,cpp}            # String helpers and miscellaneous
│
├── External/           # Third-party libraries (vendored)
│   └── tinyxml2.{h,cpp}         # XML parser
│
├── IO/                 # File input/output operations
│   └── FileIO.{h,cpp}
│
├── Model/              # Data models representing domain concepts
│   ├── Concert.{h,cpp}
│   ├── MusicalPiece.{h,cpp}
│   ├── Musician.{h,cpp}
│   ├── Rehearsal.{h,cpp}
│   ├── Score.{h,cpp}
│   ├── Timestamped.{h,cpp}
│   └── Model.{h,cpp}             # Aggregates models or supports logic
│
├── View/               # TUI views and form-based interfaces
│   ├── concert_info_form.{h,cpp}
│   ├── ConcertSummaryView.{h,cpp}
│   ├── MainMenuView.{h,cpp}
│   ├── MenuBar.{h,cpp}
│   ├── musician_form.{h,cpp}
│   ├── pieceForm.{h,cpp}
│   ├── rehearsal_form.{h,cpp}
│   ├── ScoreSelectView.{h,cpp}
│   ├── SelectionView.{h,cpp}
│   └── VimView.{h,cpp}
│
├── View.h              # Base interface or helper for View components
├── main.cpp            # Application entry point
├── makefile            # Build configuration
└── token.txt           # (Unknown use; possibly dev-related)
```

---

## Build Instructions

### Requirements:

- A C++17-compatible compiler (e.g. g++ >= 9)
- ncurses
- make

### To build:

```bash
make
```

### To clean:

```bash
make clean
```

---

## Notes

- Views use ncurses for TUI interaction.
- `tinyxml2` is used for reading/writing XML files.
- Form input is handled with `form.h` (ncurses form API).

---

## TODO / Future Ideas

- Add unit tests (possibly with GoogleTest)
- Improve error handling during form input
- Add support for exporting to PDF or CSV
- Refactor large controllers into smaller subcomponents

---

## Author

- Eugenio Giovine
