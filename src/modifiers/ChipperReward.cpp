#include "ChipperReward.h"
#include <random>
#include <iostream>

void ChipperReward::apply(ScoreContext& context) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(20, 80);
    int bonus = dist(rng);
    context.chips += bonus;
    std::cout << "  [Chipper Reward] Random bonus: +" << bonus << " chips\n";
}

std::string ChipperReward::getName() const {
    return "Chipper Reward";
}

std::string ChipperReward::getDescription() const {
    return "Adds a random +20 to +80 chips";
}

int ChipperReward::getCost() const {
    return 6;
}

