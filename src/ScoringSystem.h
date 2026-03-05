#ifndef SCORINGSYSTEM_H
#define SCORINGSYSTEM_H

#include "Card.h"
#include "ScoreContext.h"
#include "HandEvaluator.h"
#include <vector>

class ScoringSystem {
public:
    ScoreContext calculate(const std::vector<Card>& cards);
};

#endif
