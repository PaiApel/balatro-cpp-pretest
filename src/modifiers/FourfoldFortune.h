#ifndef FOUTFOLDFORTUNE_H
#define FOUTFOLDFORTUNE_H
#include "IModifier.h"

class FourfoldFortune : public IModifier {
public:
    void apply(ScoreContext& context) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getCost() const override;
};

#endif

