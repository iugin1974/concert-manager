// ConcertSummaryView.h
#pragma once

#include "Concert.h"

class ConcertSummaryView {
public:
    void show(const Concert& concert);

private:
void saveToFile(const Concert& concert);
void viewProgram(const Concert& concert) const;
};

