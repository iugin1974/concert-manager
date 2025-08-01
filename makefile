# Nome dell'eseguibile
TARGET = concertmanager

# Compilatore
CXX = clang++

# Opzioni di compilazione
CXXFLAGS = -std=c++17 -Wall -Wextra -g -fsanitize=address -fno-omit-frame-pointer \
           -I./Model -I./View -I./Controller -I./Core -I./IO -I./External

# File sorgente con percorsi relativi
SRCS = main.cpp \
       Model/Musician.cpp Model/MusicalPiece.cpp Model/Concert.cpp Model/Rehearsal.cpp Model/Score.cpp Model/Timestamped.cpp Model/Model.cpp \
       View/musician_form.cpp View/concert_info_form.cpp View/MainMenuView.cpp View/ConcertSummaryView.cpp View/pieceForm.cpp View/rehearsal_form.cpp \
       View/MenuBar.cpp View/SelectionView.cpp View/ScoreSelectView.cpp View/VimView.cpp View/MuttView.cpp \
       Controller/ConcertController.cpp \
       Core/Utils.cpp Core/logMessage.cpp Core/AbookImporter.cpp \
       IO/FileIO.cpp \
       External/tinyxml2.cpp

# File oggetto corrispondenti
OBJS = $(SRCS:.cpp=.o)

# Regola principale
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lncurses -lmenu -lform 2>&1 | c++filt

# Regola per compilare i .o dagli .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

install:
	cp $(TARGET) /usr/bin/
