#include "FourfoldFortune.h"

void FourfoldFortune::apply(ScoreContext& context) {
    context.mult *= 4;
}

std::string FourfoldFortune::getName() const {
    return "Fourfold Fortune";
}

std::string FourfoldFortune::getDescription() const {
    return "Quadruples your multiplier (Mult x4)!";
}

int FourfoldFortune::getCost() const {
    return 20;
}

