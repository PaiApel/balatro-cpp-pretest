#ifndef MODIFIERFACTORY_H
#define MODIFIERFACTORY_H

#include "modifiers/IModifier.h"
#include <string>

class ModifierFactory {
public:
    static IModifier* createModifier(const std::string& type);
    static void printAvailableModifiers();
};

#endif
