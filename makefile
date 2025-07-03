# Nome dell'eseguibile
TARGET = concerti_tui

# Compilatore
CXX = g++

# Opzioni di compilazione
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# File sorgente
SRCS = main.cpp Musician.cpp MusicalPiece.cpp Concert.cpp musician_form.cpp concert_form.cpp ConcertFormView.cpp ConcertController.cpp MainMenuView.cpp ConcertSummaryView.cpp Utils.cpp PopupMenu.cpp logMessage.cpp pieceForm.cpp

# File oggetto
OBJS = $(SRCS:.cpp=.o)

# Regola principale
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lncurses -lform 2>&1 | c++filt

clean:
	rm -f $(OBJS) $(TARGET)
