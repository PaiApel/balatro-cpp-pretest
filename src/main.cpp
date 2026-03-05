#include <iostream>
#include <algorithm>
#include <random>
#include "Deck.h"
#include "ScoringSystem.h"
#include "ModifierFactory.h"
#include "ShopSystem.h"

int main() {
    std::cout << "=== Shop System Test ===\n\n";

    int gold = 20;
    std::vector<IModifier*> activeModifiers;

    // Open shop
    ShopSystem shop;
    shop.openShop(gold, activeModifiers);

    // Show what we bought
    std::cout << "\n-- Active Modifiers --\n";
    if (activeModifiers.empty()) {
        std::cout << "  None\n";
    }
    else {
        for (IModifier* m : activeModifiers) {
            std::cout << "  - " << m->getName() << " (" << m->getDescription() << ")\n";
        }
    }

    // Score a random hand with bought modifiers
    std::cout << "\n-- Scoring with Purchased Modifiers --\n";
    Deck deck;
    deck.shuffle();

    std::vector<Card> drawn = deck.drawMultiple(8);
    std::cout << "Drawn Hand (8 cards):\n";
    for (int i = 0; i < (int)drawn.size(); i++) {
        std::cout << "  " << (i+1) << ". " << drawn[i].toString() << "\n";
    }

    std::vector<int> indices = {0,1,2,3,4,5,6,7};
    std::mt19937 rng(std::random_device{}());
    std::shuffle(indices.begin(), indices.end(), rng);

    std::vector<Card> hand;
    std::cout << "\nSelected 5 Cards:\n";
    for (int i = 0; i < 5; i++) {
        hand.push_back(drawn[indices[i]]);
        std::cout << "  " << drawn[indices[i]].toString() << "\n";
    }

    std::cout << "\n";
    ScoringSystem scorer;
    scorer.calculate(hand, activeModifiers);

    // Clean up
    for (IModifier* m : activeModifiers) {
        delete m;
    }
    activeModifiers.clear();

    return 0;
}
