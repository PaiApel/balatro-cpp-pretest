#include "ShopSystem.h"
#include <iostream>

ShopSystem::ShopSystem() {
    buildShop();
}

void ShopSystem::buildShop() {
    items.clear();
    items.push_back({"DoubleTrouble", "Double Trouble", "Doubles your multiplier (Mult x2)", 8, false});
    items.push_back({"PocketChange", "Pocket Change", "Adds +50 flat chips", 6, false});
}

void ShopSystem::displayShop(int gold) const {
    std::cout << "\n========================================\n";
    std::cout << "               S H O P\n";
    std::cout << "========================================\n";
    std::cout << "  Gold: " << gold << "\n\n";
    for (int i = 0; i < (int)items.size(); i++) {
        if (items[i].sold) {
            std::cout << "  " << (i+1) << ". [SOLD] "  << items[i].displayName << "\n";
        }
        else {
            std::cout << "  " << (i+1) << ". " << items[i].displayName << " - " << items[i].description << " (Cost: " << items[i].cost << " gold)\n";
        }
    }
    std::cout << "  0. Leave Shop\n";
    std::cout << "========================================\n";
}

bool ShopSystem::buyItem(int index, int& gold, std::vector<IModifier*>& activeModifiers) {
    ShopItem& item = items[index];

    if (item.sold) {
        std::cout << "  This item is already sold!\n";
        return false;
    }
    if (gold < item.cost) {
        std::cout << "  Not enough gold! (Need " << item.cost
                  << ", have " << gold << ")\n";
        return false;
    }

    IModifier* modifier = ModifierFactory::createModifier(item.modifierType);
    if (!modifier) {
        std::cout << "  Failed to create modifier!\n";
        return false;
    }

    gold -= item.cost;
    item.sold = true;
    activeModifiers.push_back(modifier);

    std::cout << "  Bought: " << item.displayName << "! (-" << item.cost << " gold)\n";
    std::cout << "  Remaining gold: " << gold << "\n";
    return true;
}

void ShopSystem::openShop(int& gold, std::vector<IModifier*>& activeModifiers) {
    while (true) {
        displayShop(gold);
        std::cout << "  Enter choice: ";
        int choice;
        std::cin >> choice;

        if (choice == 0) {
            std::cout << "  Leaving shop...\n";
            break;
        }

        if (choice < 1 || choice > (int)items.size()) {
            std::cout << "  Invalid choice!\n";
            continue;
        }

        buyItem(choice - 1, gold, activeModifiers);
    }
}
