#include "PocketChange.h"

void PocketChange::apply(ScoreContext& context) {
    context.chips += 50;
}

std::string PocketChange::getName() const {
    return "PocketChange";
}

std::string PocketChange::getDescription() const {
    return "Adds +50 flat chips to your score";
}

int PocketChange::getCost() const {
    return 6;
}
