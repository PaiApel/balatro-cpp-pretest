#include "SmallBonus.h"

void SmallBonus::apply(ScoreContext& context) {
    context.mult += 3;
}

std::string SmallBonus::getName() const {
    return "Small Bonus";
}

std::string SmallBonus::getDescription() const {
    return "Adds a +3 flat mult";
}

int SmallBonus::getCost() const {
    return 6;
}
