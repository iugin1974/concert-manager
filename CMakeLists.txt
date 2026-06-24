#
# Build normale (Debug + ASan, equivalente a 'make')
# mkdir build && cd build
# cmake ..
# make -j$(nproc)
# 
# Build per Valgrind (equivalente a 'make valgrind')
# mkdir build-valgrind && cd build-valgrind
# cmake -DVALGRIND=ON ..
# make -j$(nproc)
#
# Install
# sudo make install
#

cmake_minimum_required(VERSION 3.16)
project(concertmanager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Sorgenti
# ---------------------------------------------------------------------------
set(SRCS
    main.cpp
    Model/Musician.cpp
    Model/MusicalPiece.cpp
    Model/Concert.cpp
    Model/Rehearsal.cpp
    Model/Score.cpp
    Model/Timestamped.cpp
    Model/Model.cpp
    View/musician_form.cpp
    View/concert_info_form.cpp
    View/MainMenuView.cpp
    View/ConcertSummaryView.cpp
    View/pieceForm.cpp
    View/rehearsal_form.cpp
    View/MenuBar.cpp
    View/SelectionView.cpp
    View/ListSelectView.cpp
    View/VimView.cpp
    View/MuttView.cpp
    Controller/ConcertController.cpp
    Core/Utils.cpp
    Core/logMessage.cpp
    Core/AbookImporter.cpp
    IO/FileIO.cpp
    IO/HTML_Exporter.cpp
    IO/CSV_Exporter.cpp
    External/tinyxml2.cpp
)

# ---------------------------------------------------------------------------
# Eseguibile
# ---------------------------------------------------------------------------
add_executable(concertmanager ${SRCS})

# ---------------------------------------------------------------------------
# Include directories
# ---------------------------------------------------------------------------
target_include_directories(concertmanager PRIVATE
    Model
    View
    Controller
    Core
    IO
    External
)

# ---------------------------------------------------------------------------
# Librerie (ncurses wide)
# ---------------------------------------------------------------------------
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)
message(STATUS "CURSES_LIBRARIES: ${CURSES_LIBRARIES}")
message(STATUS "CURSES_INCLUDE_DIRS: ${CURSES_INCLUDE_DIRS}")
target_link_libraries(concertmanager PRIVATE
    ${CURSES_LIBRARIES}
    menuw
    formw
)
target_include_directories(concertmanager PRIVATE ${CURSES_INCLUDE_DIRS})

# ---------------------------------------------------------------------------
# Flags comuni
# ---------------------------------------------------------------------------
target_compile_options(concertmanager PRIVATE
    -Wall
    -Wextra
    -fno-omit-frame-pointer
)

# ---------------------------------------------------------------------------
# Build type: default Debug (equivalente a -g + ASan)
# ---------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ---------------------------------------------------------------------------
# Preset: ASan (default, equivalente al tuo target 'all')
# ---------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(concertmanager PRIVATE
        -g
        -fsanitize=address
    )
    target_link_options(concertmanager PRIVATE
        -fsanitize=address
    )
endif()

# ---------------------------------------------------------------------------
# Preset: Valgrind  →  cmake -DVALGRIND=ON ..
# Usa g++ e disabilita ASan (incompatibile con valgrind)
# ---------------------------------------------------------------------------
option(VALGRIND "Build for Valgrind (g++, no ASan, gdwarf-4)" OFF)

if(VALGRIND)
    set(CMAKE_CXX_COMPILER "g++" CACHE STRING "" FORCE)
    target_compile_options(concertmanager PRIVATE
        -g
        -gdwarf-4
        -O0
    )
    # Rimuove -fsanitize=address se era già aggiunto
    target_link_options(concertmanager PRIVATE)
endif()

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------
install(TARGETS concertmanager DESTINATION /usr/bin)
