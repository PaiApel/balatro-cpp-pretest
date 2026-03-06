#include "ModifierFactory.h"
#include "modifiers/ChipIn.h"
#include "modifiers/PocketChange.h"
#include "modifiers/MegaPayout.h"
#include "modifiers/SmallBonus.h"
#include "modifiers/LuckySeven.h"
#include "modifiers/MidasTouch.h"
#include "modifiers/DoubleTrouble.h"
#include "modifiers/TripleThreat.h"
#include "modifiers/FourfoldFortune.h"
#include "modifiers/ChipperReward.h"
#include "modifiers/Fortune'sFavor.h"
#include <iostream>

IModifier* ModifierFactory::createModifier(const std::string& type) {
    if (type == "ChipIn") {
        return new ChipIn();
    }
    if (type == "PocketChange") {
        return new PocketChange();
    }
    if (type == "MegaPayout") {
        return new MegaPayout();
    }
    if (type == "SmallBonus") {
        return new SmallBonus();
    }
    if (type == "LuckySeven") {
        return new LuckySeven();
    }
    if (type == "MidasTouch") {
        return new MidasTouch();
    }
    if (type == "DoubleTrouble") {
        return new DoubleTrouble();
    }
    if (type == "TripleThreat") {
        return new TripleThreat();
    }
    if (type == "FourfoldFortune") {
        return new FourfoldFortune();
    }
    if (type == "ChipperReward") {
        return new ChipperReward();
    }
    if (type == "FortunesFavor") {
        return new FortunesFavor();
    }

    std::cout << "  [ModifierFactory] Unknown modifier type: " << type << "\n";
    return nullptr;
}

void ModifierFactory::printAvailableModifiers() {
    std::cout << "  Available Modifiers:\n";
    std::cout << "  1. DoubleTrouble - Doubles your multiplier (Cost: 8)\n";
    std::cout << "  2. PocketChange - Adds +50 flat chips (Cost: 6)\n";
    std::cout << "  3. ChipperReward - Adds random +20 to +80 chips (Cost: 5)\n";
    std::cout << "  4. Fortune'sFavor - Adds random +2 to +16 mult (Cost: 5)\n";
}
