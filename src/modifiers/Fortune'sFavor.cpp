#include "Fortune'sFavor.h"
#include <random>
#include <iostream>

void FortunesFavor::apply(ScoreContext& context) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(2, 16);
    int bonus = dist(rng);
    context.mult += bonus;
    std::cout << "  [Fortune's Favor] Random bonus: +" << bonus << " mult\n";
}

std::string FortunesFavor::getName() const {
    return "Fortune's Favor";
}

std::string FortunesFavor::getDescription() const {
    return "Adds a random +2 to +16 mult";
}

int FortunesFavor::getCost() const {
    return 7;
}
