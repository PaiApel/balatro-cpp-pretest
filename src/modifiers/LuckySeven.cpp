#include "LuckySeven.h"

void LuckySeven::apply(ScoreContext& context) {
    context.mult += 7;
}

std::string LuckySeven::getName() const {
    return "Lucky 7";
}

std::string LuckySeven::getDescription() const {
    return "Adds a +7 flat mult";
}

int LuckySeven::getCost() const {
    return 8;
}

