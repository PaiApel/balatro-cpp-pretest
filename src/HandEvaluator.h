#ifndef HANDEVALUATOR_H
#define HANDEVALUATOR_H

#include "Card.h"
#include "ScoreContext.h"
#include <vector>
#include <string>
#include <map>

class HandEvaluator {
public:
    struct HandDefinition {
        std::string handName;
        int baseChips;
        int baseMult;
    };

    struct HandResult {
        std::string handName;
        int baseChips;
        int baseMult;
        std::vector<Card> scoringCards;
    };

    static const std::vector<HandDefinition> handDefinitions;
    static HandResult evaluate(const std::vector<Card>& cards);

private:
    static bool isFlush(const std::vector<Card>& cards);
    static bool isStraight(const std::vector<Card>& cards);
    static std::map<int, std::vector<Card>> getRankGroups(const std::vector<Card>& cards);
};

#endif
