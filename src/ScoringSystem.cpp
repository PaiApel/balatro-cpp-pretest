#include "ScoringSystem.h"
#include <iostream>

ScoreContext ScoringSystem::calculate(const std::vector<Card>& cards, const std::vector<IModifier*>& modifiers) {
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

    ScoreContext ctx(totalChips, result.baseMult);
    std::cout << "  Total Chips: " << ctx.chips << " | Mult: " << ctx.mult << "\n";

    // Apply modifiers
    if (!modifiers.empty()) {
        std::cout << "\n  -- Applying Modifiers --\n";
        for (IModifier* m : modifiers) {
            m->apply(ctx);
            std::cout << "  [" << m->getName() << "] " << "-> Chips: " << ctx.chips << " | Mult: " << ctx.mult << "\n";
        }
    }

    std::cout << "\n  Final Score: " << ctx.finalScore() << "\n";
    return ctx;
}
