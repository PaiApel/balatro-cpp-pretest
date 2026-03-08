#include "MidasTouch.h"

void MidasTouch::apply(ScoreContext& context) {
    context.mult += 12;
}

std::string MidasTouch::getName() const {
    return "Midas Touch";
}

std::string MidasTouch::getDescription() const {
    return "Adds a +12 flat mult";
}

int MidasTouch::getCost() const {
    return 14;
}

