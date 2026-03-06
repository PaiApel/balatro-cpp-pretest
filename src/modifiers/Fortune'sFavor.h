#ifndef FORTUNESFAVOR_H
#define FORTUNESFAVOR_H

#include "IModifier.h"

class FortunesFavor : public IModifier {
public:
    void apply(ScoreContext& context) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getCost() const override;
};

#endif
