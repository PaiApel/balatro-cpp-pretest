#ifndef SMALLBONUS_H
#define SMALLBONUS_H

#include "IModifier.h"

class SmallBonus : public IModifier {
public:
    void apply(ScoreContext& context) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getCost() const override;
};

#endif
