#ifndef MIDASTOUCH_H
#define MIDASTOUCH_H

#include "IModifier.h"

class MidasTouch : public IModifier {
public:
    void apply(ScoreContext& context) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getCost() const override;
};

#endif

