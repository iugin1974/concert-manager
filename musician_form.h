#pragma once

#include <ncurses.h>
#include <form.h>
#include <string>
#include "Musician.h"
#include <vector>
// Mostra un form per inserire o modificare un Musician
// Se passata una pointer valida, i dati sono precompilati
// ritorna true se l'utente ha riempito i campi in maniera valida
// e quindi il musicista può essere salveto
bool runMusicianForm(const Musician* existing, Musician&);

int runChoiceMusicianForm(std::vector<Musician>&);
