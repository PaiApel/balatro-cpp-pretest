#ifndef IMODIFIER_H
#define IMODIFIER_H

#include "../ScoreContext.h"
#include <string>

class IModifier {
public:
    virtual void apply(ScoreContext& context) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getCost() const = 0;
    virtual ~IModifier() {}
};

#endif
