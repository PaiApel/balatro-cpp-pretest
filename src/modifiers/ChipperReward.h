#ifndef CHIPPERREWARD_H
#define CHIPPERREWARD_H

#include "IModifier.h"

class ChipperReward : public IModifier {
public:
    void apply(ScoreContext& context) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getCost() const override;
};

#endif

