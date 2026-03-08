#include <iostream>
#include "RunSession.h"

int main() {
    std::cout << "========================================\n";
    std::cout << "       Welcome to the Game!\n";
    std::cout << "========================================\n";
    std::cout << "  A card-based roguelike game\n\n";
    std::cout << "  HOW TO PLAY:\n";
    std::cout << "  - Beat the score target each round\n";
    std::cout << "  - Play poker hands to score points\n";
    std::cout << "  - Buy modifiers to boost your score\n";
    std::cout << "  - Survive as long as you can!\n\n";
    std::cout << "  CONTROLS:\n";
    std::cout << "  - [numbers]   = play selected cards\n";
    std::cout << "  - [numbers] D = discard selected cards\n";
    std::cout << "  - RS = sort by rank   | SS = sort by suit\n";
    std::cout << "  - H  = hand reference | Q  = quit\n\n";
    std::cout << "  SCORING:\n";
    std::cout << "  - Score = (Base Chips + Card Chips) x Mult\n";
    std::cout << "  - Modifiers applied after base score\n";
    std::cout << "  - Order of modifiers matters!\n";
    std::cout << "========================================\n";
    std::cout << "  Press ENTER to start your run...\n";
    std::cout << "========================================\n";

    // Wait for Enter
    std::string dummy;
    std::getline(std::cin, dummy);

    RunSession run;
    run.startRun();
    return 0;
}
