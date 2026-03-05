#include <iostream>
#include <algorithm>
#include <random>
#include "Deck.h"
#include "ScoringSystem.h"
#include "modifiers/DoubleTrouble.h"
#include "modifiers/PocketChange.h"

int main() {
    std::cout << "=== Modifier Test ===\n\n";

    Deck deck;
    deck.shuffle();

    // Draw 8 cards
    std::vector<Card> drawn = deck.drawMultiple(8);
    std::cout << "-- Drawn Hand (8 cards) --\n";
    for (int i = 0; i < (int)drawn.size(); i++) {
        std::cout << "  " << (i+1) << ". " << drawn[i].toString() << " (Chips: " << drawn[i].getChipValue() << ")\n";
    }

    // Pick 5 random cards
    std::vector<int> indices = {0,1,2,3,4,5,6,7};
    std::mt19937 rng(std::random_device{}());
    std::shuffle(indices.begin(), indices.end(), rng);

    std::vector<Card> hand;
    std::cout << "\n-- Selected 5 Cards --\n";
    for (int i = 0; i < 5; i++) {
        hand.push_back(drawn[indices[i]]);
        std::cout << "  " << drawn[indices[i]].toString() << "\n";
    }

    // Test without modifiers
    std::cout << "\n-- Score WITHOUT Modifiers --\n";
    ScoringSystem scorer;
    scorer.calculate(hand, {});

    // Test with modifiers
    std::cout << "\n-- Score WITH Modifiers --\n";
    DoubleTrouble dt;
    PocketChange pc;
    std::vector<IModifier*> modifiers = {&pc, &dt};
    scorer.calculate(hand, modifiers);

    return 0;
}
