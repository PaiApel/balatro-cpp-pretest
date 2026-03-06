#include "ChipIn.h"

void ChipIn::apply(ScoreContext& context) {
    context.chips += 25;
}

std::string ChipIn::getName() const {
    return "Chip In";
}

std::string ChipIn::getDescription() const {
    return "Adds +25 flat chips to your score";
}

int ChipIn::getCost() const {
    return 5;
}

