#ifndef POCKETCHANGE_H
#define POCKETCHANGE_H

#include "IModifier.h"

class PocketChange : public IModifier {
public:
    void apply(ScoreContext& context) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getCost() const override;
};

#endif
