#include "ModifierFactory.h"
#include "modifiers/DoubleTrouble.h"
#include "modifiers/PocketChange.h"
#include <iostream>

IModifier* ModifierFactory::createModifier(const std::string& type) {
    if (type == "DoubleTrouble") {
        return new DoubleTrouble();
    }
    if (type == "PocketChange") {
        return new PocketChange();
    }

    std::cout << "  [ModifierFactory] Unknown modifier type: " << type << "\n";
    return nullptr;
}

void ModifierFactory::printAvailableModifiers() {
    std::cout << "  Available Modifiers:\n";
    std::cout << "  1. DoubleTrouble - Doubles your multiplier (Cost: 8)\n";
    std::cout << "  2. PocketChange - Adds +50 flat chips (Cost: 6)\n";
}
