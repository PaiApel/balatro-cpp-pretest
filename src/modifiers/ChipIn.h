#ifndef CHIPIN_H
#define CHIPIN_H

#include "IModifier.h"

class ChipIn : public IModifier {
public:
    void apply(ScoreContext& context) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getCost() const override;
};

#endif

