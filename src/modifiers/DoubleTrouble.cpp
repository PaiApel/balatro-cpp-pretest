#include "DoubleTrouble.h"

void DoubleTrouble::apply(ScoreContext& context) {
    context.mult *= 2;
}

std::string DoubleTrouble::getName() const {
    return "Double Trouble";
}

std::string DoubleTrouble::getDescription() const {
    return "Doubles your multiplier (Mult x2)";
}

int DoubleTrouble::getCost() const {
    return 8;
}
