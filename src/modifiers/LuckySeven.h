#ifndef LUCKYSEVEN_H
#define LUCKYSEVEN_H

#include "IModifier.h"

class LuckySeven : public IModifier {
public:
    void apply(ScoreContext& context) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getCost() const override;
};

#endif

