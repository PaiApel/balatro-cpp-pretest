#include "HandEvaluator.h"
#include <algorithm>
#include <map>

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

    // Straight Flush
    if (straight && flush) {
        return {"Straight Flush", 100, 8, cards};
    }

    // Four of a Kind
    if (counts[0] == 4) {
        return {"Four of a Kind", 60, 7, collectGroups(1)};
    }

    // Full House
    if (counts[0] == 3 && counts.size() > 1 && counts[1] == 2) {
        return {"Full House", 40, 4, cards};
    }

    // Flush
    if (flush) {
        return {"Flush", 35, 4, cards};
    }

    // Straight
    if (straight) {
        return {"Straight", 30, 4, cards};
    }

    // Three of a Kind
    if (counts[0] == 3) {
        return {"Three of a Kind", 30, 3, collectGroups(1)};
    }

    // Two Pair
    if (counts[0] == 2 && counts.size() > 1 && counts[1] == 2) {
        return {"Two Pair", 20, 2, collectGroups(2)};
    }

    // Pair
    if (counts[0] == 2) {
        return {"Pair", 10, 2, collectGroups(1)};
    }

    // High Card
    std::vector<Card> highCard;
    Card highest = cards[0];
    for (const Card& c : cards) {
        if (static_cast<int>(c.getRank()) > static_cast<int>(highest.getRank())) {
            highest = c;
        }
    }
    highCard.push_back(highest);
    return {"High Card", 5, 1, highCard};
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
