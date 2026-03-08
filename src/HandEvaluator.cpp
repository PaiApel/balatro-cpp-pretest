#include "HandEvaluator.h"
#include <algorithm>
#include <map>

// Static hand definitions — single source of truth
const std::vector<HandEvaluator::HandDefinition> HandEvaluator::handDefinitions = {
    {"High Card",       8,   1},
    {"Pair",            15,  1},
    {"Two Pair",        25,  2},
    {"Three of a Kind", 35,  2},
    {"Straight",        40,  3},
    {"Flush",           45,  3},
    {"Full House",      50,  3},
    {"Four of a Kind",  70,  4},
    {"Straight Flush",  120, 5}
};

// Helper to find definition by name
static const HandEvaluator::HandDefinition& findDef(const std::string& name) {
    for (const auto& def : HandEvaluator::handDefinitions)
        if (def.handName == name) return def;
    return HandEvaluator::handDefinitions[0]; // fallback to High Card
}

HandEvaluator::HandResult HandEvaluator::evaluate(const std::vector<Card>& cards) {
    bool flush    = isFlush(cards);
    bool straight = isStraight(cards);
    std::map<int, std::vector<Card>> rankGroups = getRankGroups(cards);

    // Build sorted groups by count descending
    std::vector<std::pair<int, std::vector<Card>>> groups(rankGroups.begin(), rankGroups.end());
    std::sort(groups.begin(), groups.end(),
        [](const auto& a, const auto& b) {
            return a.second.size() > b.second.size();
        });

    std::vector<int> counts;
    for (auto& g : groups) {
        counts.push_back(g.second.size());
    }

    // Helper to collect scoring cards from top N groups
    auto collectGroups = [&](int n) {
        std::vector<Card> result;
        for (int i = 0; i < n && i < (int)groups.size(); i++) {
            for (const Card& c : groups[i].second) {
                result.push_back(c);
            }
        }
        return result;
    };

    // Determine hand name and scoring cards
    std::string handName;
    std::vector<Card> scoringCards;

    if (straight && flush) {
        handName = "Straight Flush";
        scoringCards = cards;
    }
    else if (counts[0] == 4) {
        handName = "Four of a Kind";
        scoringCards = collectGroups(1);
    }
    else if (counts[0] == 3 && counts.size() > 1 && counts[1] == 2) {
        handName = "Full House";
        scoringCards = cards;
    }
    else if (flush) {
        handName = "Flush";
        scoringCards = cards;
    }
    else if (straight) {
        handName = "Straight";
        scoringCards = cards;
    }
    else if (counts[0] == 3) {
        handName = "Three of a Kind";
        scoringCards = collectGroups(1);
    }
    else if (counts[0] == 2 && counts.size() > 1 && counts[1] == 2) {
        handName = "Two Pair";
        scoringCards = collectGroups(2);
    }
    else if (counts[0] == 2) {
        handName = "Pair";
        scoringCards = collectGroups(1);
    }
    else {
        handName = "High Card";
        Card highest = cards[0];
        for (const Card& c : cards) {
            if (static_cast<int>(c.getRank()) > static_cast<int>(highest.getRank())){
                highest = c;
            }
        }
        scoringCards = {highest};
    }

    // Look up definition
    const HandDefinition& def = findDef(handName);
    return {def.handName, def.baseChips, def.baseMult, scoringCards};
}

bool HandEvaluator::isFlush(const std::vector<Card>& cards) {
    if (cards.size() < 5) {
        return false;
    }
    Suit s = cards[0].getSuit();
    for (const Card& c : cards) {
        if (c.getSuit() != s) return false;
    }
    return true;
}

bool HandEvaluator::isStraight(const std::vector<Card>& cards) {
    if (cards.size() < 5) {
        return false;
    }
    std::vector<int> ranks;
    for (const Card& c : cards) {
        ranks.push_back(static_cast<int>(c.getRank()));
    }
    std::sort(ranks.begin(), ranks.end());

    bool normal = true;
    for (int i = 1; i < (int)ranks.size(); i++) {
        if (ranks[i] != ranks[i-1] + 1) {
            normal = false;
            break;
        }
    }
    if (normal) {
        return true;
    }

    // Ace-low straight (A-2-3-4-5)
    if (ranks.back() == static_cast<int>(Rank::ACE)) {
        ranks.pop_back();
        ranks.insert(ranks.begin(), 1);
        bool aceLow = true;
        for (int i = 1; i < (int)ranks.size(); i++) {
            if (ranks[i] != ranks[i-1] + 1) {
                aceLow = false;
                break;
            }
        }
        return aceLow;
    }
    return false;
}

std::map<int, std::vector<Card>> HandEvaluator::getRankGroups(const std::vector<Card>& cards) {
    std::map<int, std::vector<Card>> groups;
    for (const Card& c : cards) {
        groups[static_cast<int>(c.getRank())].push_back(c);
    }
    return groups;
}
