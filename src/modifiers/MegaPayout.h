#ifndef MEGAPAYOUT_H
#define MEGAPAYOUT_H

#include "IModifier.h"

class MegaPayout : public IModifier {
public:
    void apply(ScoreContext& context) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getCost() const override;
};

#endif

