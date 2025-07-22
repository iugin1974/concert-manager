# Nome dell'eseguibile
TARGET = concertmanager

# Compilatore
CXX = clang++

# Opzioni di compilazione
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# File sorgente
SRCS = main.cpp Musician.cpp MusicalPiece.cpp Concert.cpp musician_form.cpp concert_info_form.cpp ConcertController.cpp MainMenuView.cpp ConcertSummaryView.cpp Utils.cpp logMessage.cpp pieceForm.cpp Rehearsal.cpp rehearsal_form.cpp MenuBar.cpp SelectionView.cpp ScoreSelectView.cpp File.cpp Timestamped.cpp Model.cpp VimView.cpp Score.cpp tinyxml2.cpp

# File oggetto
OBJS = $(SRCS:.cpp=.o)

# Regola principale
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lncurses -lmenu -lform 2>&1 | c++filt

clean:
	rm -f $(OBJS) $(TARGET)

install:
	cp $(TARGET) /usr/bin/
