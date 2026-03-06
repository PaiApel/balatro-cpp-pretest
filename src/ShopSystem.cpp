#include "ShopSystem.h"
#include <iostream>
#include <algorithm>
#include <random>

const int ShopSystem::SHOP_SIZE;

ShopSystem::ShopSystem() {
    buildItemPool();
    refreshShop();
}

void ShopSystem::buildItemPool() {
    allItems.clear();
    allItems.push_back({"ChipIn", "Chip In", "Adds +25 flat chips", 5, false});
    allItems.push_back({"PocketChange", "Pocket Change", "Adds +50 flat chips", 7, false});
    allItems.push_back({"MegaPayout", "Mega Payout", "Adds +90 flat chips", 9, false});
    allItems.push_back({"SmallBonus", "Small Bonus", "Adds a flat +3 mult", 6, false});
    allItems.push_back({"LuckySeven", "Lucky Seven", "Adds a flat +7 mult", 8, false});
    allItems.push_back({"MidasTouch", "Midas Touch", "Adds a flat +12 mult", 10, false});
    allItems.push_back({"DoubleTrouble", "Double Trouble", "Doubles your multiplier (Mult x2)", 10, false});
    allItems.push_back({"TripleThreat", "Triple Threat", "Triples your multiplier (Mult x3)", 12, false});
    allItems.push_back({"FourfoldFortune", "Fourfold Fortune", "Quadruples your multiplier (Mult x4)", 14, false});
    allItems.push_back({"ChipperReward", "Chipper Reward", "Adds random +20 to +80 chips", 6, false});
    allItems.push_back({"FortunesFavor", "Fortune's Favor", "Adds random +2 to +16 mult", 7, false});
}

void ShopSystem::refreshShop() {
    // Shuffle a copy of the pool and pick SHOP_SIZE items
    std::vector<ShopItem> pool = allItems;
    std::mt19937 rng(std::random_device{}());
    std::shuffle(pool.begin(), pool.end(), rng);

    currentItems.clear();
    int count = std::min(SHOP_SIZE, (int)pool.size());
    for (int i = 0; i < count; i++) {
        pool[i].sold = false; // Reset sold status
        currentItems.push_back(pool[i]);
    }
}

void ShopSystem::displayShop(int gold) const {
    std::cout << "\n========================================\n";
    std::cout << "               S H O P\n";
    std::cout << "========================================\n";
    std::cout << "  Gold: " << gold << "\n\n";
    for (int i = 0; i < (int)currentItems.size(); i++) {
        if (currentItems[i].sold) {
            std::cout << "  " << (i+1) << ". [SOLD] "  << currentItems[i].displayName << "\n";
        }
        else {
            std::cout << "  " << (i+1) << ". " << currentItems[i].displayName << " - " << currentItems[i].description << " (Cost: " << currentItems[i].cost << " gold)\n";
        }
    }
    std::cout << "  0. Leave Shop\n";
    std::cout << "========================================\n";
}

bool ShopSystem::buyItem(int index, int& gold, std::vector<IModifier*>& activeModifiers) {
    ShopItem& item = currentItems[index];

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

        if (choice < 1 || choice > (int)currentItems.size()) {
            std::cout << "  Invalid choice!\n";
            continue;
        }

        buyItem(choice - 1, gold, activeModifiers);
    }
}
