#include <iostream>
#include "Deck.h"
#include "ScoringSystem.h"
#include <algorithm>
#include <random>

int main() {
    std::cout << "=== Scoring Test ===\n\n";

    Deck deck;
    deck.shuffle();

    // Draw 8 cards
    std::vector<Card> drawn = deck.drawMultiple(8);

    std::cout << "-- Drawn Hand (8 cards) --\n";
    for (int i = 0; i < (int)drawn.size(); i++) {
        std::cout << "  " << (i+1) << ". " << drawn[i].toString() << " (Chips: " << drawn[i].getChipValue() << ")\n";
    }

    // Pick 5 random indices from the 8
    std::vector<int> indices = {0, 1, 2, 3, 4, 5, 6, 7};
    std::mt19937 rng(std::random_device{}());
    std::shuffle(indices.begin(), indices.end(), rng);

    std::vector<Card> hand;
    std::cout << "\n-- Selected 5 Cards --\n";
    for (int i = 0; i < 5; i++) {
        hand.push_back(drawn[indices[i]]);
        std::cout << "  " << drawn[indices[i]].toString() << "\n";
    }

    // Score the hand
    std::cout << "\n-- Score Calculation --\n";
    ScoringSystem scorer;
    ScoreContext ctx = scorer.calculate(hand);
    std::cout << "  Final Score: " << ctx.finalScore() << "\n";

    return 0;
}
