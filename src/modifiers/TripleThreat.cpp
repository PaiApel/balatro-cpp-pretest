#include "TripleThreat.h"

void TripleThreat::apply(ScoreContext& context) {
    context.mult *= 3;
}

std::string TripleThreat::getName() const {
    return "Triple Threat";
}

std::string TripleThreat::getDescription() const {
    return "Triples your multiplier (Mult x3)";
}

int TripleThreat::getCost() const {
    return 16;
}

