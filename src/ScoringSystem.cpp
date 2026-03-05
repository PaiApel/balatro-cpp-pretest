#include "ScoringSystem.h"
#include <iostream>

ScoreContext ScoringSystem::calculate(const std::vector<Card>& cards) {
    HandEvaluator::HandResult result = HandEvaluator::evaluate(cards);

    std::cout << "  Hand: " << result.handName << "\n";
    std::cout << "  Base Chips: " << result.baseChips << " | Base Mult: " << result.baseMult << "\n";

    int cardChips = 0;
    std::cout << "  Scoring Cards: ";
    for (const Card& c : result.scoringCards) {
        std::cout << "\n   - " << c.toString();
        cardChips += c.getChipValue();
    }
    std::cout << "\n";

    int totalChips = result.baseChips + cardChips;
    std::cout << "  Card Chips Added: " << cardChips << "\n";
    std::cout << "  Total Chips: " << totalChips << " | Mult: " << result.baseMult << "\n";

    return ScoreContext(totalChips, result.baseMult);
}
