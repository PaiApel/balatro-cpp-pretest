#include "MegaPayout.h"

void MegaPayout::apply(ScoreContext& context) {
    context.chips += 90;
}

std::string MegaPayout::getName() const {
    return "Mega Payout";
}

std::string MegaPayout::getDescription() const {
    return "Adds +90 flat chips to your score";
}

int MegaPayout::getCost() const {
    return 9;
}

