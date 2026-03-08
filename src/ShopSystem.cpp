#include "ShopSystem.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <limits>

const int ShopSystem::SHOP_SIZE;
const int ShopSystem::MODIFIER_LIMIT;

ShopSystem::ShopSystem() {
    buildItemPool();
    refreshShop();
}

void ShopSystem::buildItemPool() {
    allItems.clear();
    allItems.push_back({"ChipIn", "Chip In", "Adds +25 flat chips", 5, false});
    allItems.push_back({"PocketChange", "Pocket Change", "Adds +50 flat chips", 7, false});
    allItems.push_back({"MegaPayout", "Mega Payout", "Adds +90 flat chips", 10, false});
    allItems.push_back({"SmallBonus", "Small Bonus", "Adds a flat +3 mult", 6, false});
    allItems.push_back({"LuckySeven", "Lucky Seven", "Adds a flat +7 mult", 8, false});
    allItems.push_back({"MidasTouch", "Midas Touch", "Adds a flat +12 mult", 14, false});
    allItems.push_back({"DoubleTrouble", "Double Trouble", "Doubles your multiplier (Mult x2)", 10, false});
    allItems.push_back({"TripleThreat", "Triple Threat", "Triples your multiplier (Mult x3)", 16, false});
    allItems.push_back({"FourfoldFortune", "Fourfold Fortune", "Quadruples your multiplier (Mult x4)", 20, false});
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

void ShopSystem::displayShop(int gold, int rerollCost, const std::vector<IModifier*>& activeModifiers) const {
    int modCount = (int)activeModifiers.size();

    std::cout << "\n========================================\n";
    std::cout << "               S H O P\n";
    std::cout << "========================================\n";
    std::cout << "  Gold: " << gold << "  |  Modifiers: " << modCount << "/" << MODIFIER_LIMIT << "\n\n";
    for (int i = 0; i < (int)currentItems.size(); i++) {
        if (currentItems[i].sold) {
            std::cout << "  " << (i+1) << ". [SOLD] "  << currentItems[i].displayName << "\n";
        }
        else {
            std::cout << "  " << (i+1) << ". " << currentItems[i].displayName << " - " << currentItems[i].description << " (Cost: " << currentItems[i].cost << " gold)\n";
        }
    }
    std::cout << "  0. Leave Shop\n";
    std::cout << "  R. Reroll (" << rerollCost << " gold)\n";
    std::cout << "  M. Manage Modifiers\n";
    std::cout << "========================================\n";
}

bool ShopSystem::buyItem(int index, int& gold, std::vector<IModifier*>& activeModifiers) {
    ShopItem& item = currentItems[index];

    if (item.sold) {
        std::cout << "  This item is already sold!\n";
        return false;
    }
    if (gold < item.cost) {
        std::cout << "  Not enough gold! (Need " << item.cost << ", have " << gold << ")\n";
        return false;
    }
    if ((int)activeModifiers.size() >= MODIFIER_LIMIT) {
        std::cout << "  Modifier slots full! (" << MODIFIER_LIMIT << "/" << MODIFIER_LIMIT << ")\n";
        std::cout << "  Sell a modifier first (M to manage).\n";
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

void ShopSystem::displayModifierManager(const std::vector<IModifier*>& activeModifiers) const {
    int modCount = (int)activeModifiers.size();

    std::cout << "\n========================================\n";
    std::cout << "     M O D I F I E R S  (" << modCount << "/" << MODIFIER_LIMIT << ")\n";
    std::cout << "========================================\n";

    if (activeModifiers.empty()) {
        std::cout << "  No modifiers owned.\n";
    }
    else {
        for (int i = 0; i < modCount; i++) {
            std::cout << "  " << (i+1) << ". " << activeModifiers[i]->getName() << " - " << activeModifiers[i]->getDescription() << " (sell: " << (activeModifiers[i]->getCost() / 2) << " gold)\n";
}
    }

    std::cout << "\n  Swap : '1 3' to swap positions\n";
    std::cout << "  Sell : 'S 2' to sell modifier 2\n";
    std::cout << "  Back : '0' to return to shop\n";
    std::cout << "========================================\n";
}

void ShopSystem::openModifierManager(int& gold, std::vector<IModifier*>& activeModifiers) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true) {
        displayModifierManager(activeModifiers);
        std::cout << "  > ";

        std::string line;
        std::getline(std::cin, line);

        // Convert to uppercase
        for (char& c : line) {
            c = toupper(c);
        }

        // Back
        if (line == "0") {
            break;
        }

        // Sell: "S X"
        if (line[0] == 'S') {
            std::istringstream iss(line.substr(1));
            int idx;
            if (iss >> idx && idx >= 1 && idx <= (int)activeModifiers.size()) {
                int sellPrice = activeModifiers[idx-1]->getCost() / 2;
                std::cout << "  Sold: " << activeModifiers[idx-1]->getName() << " (+" << sellPrice << " gold)\n";
                gold += sellPrice;
                delete activeModifiers[idx-1];
                activeModifiers.erase(activeModifiers.begin() + idx - 1);
            }
            else {
                std::cout << "  Invalid sell command.\n";
            }
            continue;
        }

        // Swap: "X Y"
        std::istringstream iss(line);
        int a, b;
        if (iss >> a >> b) {
            if (a >= 1 && a <= (int)activeModifiers.size() && b >= 1 && b <= (int)activeModifiers.size() && a != b) {
                std::swap(activeModifiers[a-1], activeModifiers[b-1]);
                std::cout << "  Swapped modifier " << a << " and " << b << ".\n";
            }
            else {
                std::cout << "  Invalid swap. Enter two different valid modifier numbers.\n";
            }
            continue;
        }

        std::cout << "  Invalid command.\n";
    }
}

void ShopSystem::openShop(int& gold, std::vector<IModifier*>& activeModifiers) {
    int rerollCost = 3;

    while (true) {
        displayShop(gold, rerollCost, activeModifiers);
        std::cout << "  Enter choice: ";

        std::string input;
        std::cin >> input;

        // Convert to uppercase
        for (char& c : input) {
            c = toupper(c);
        }

        // Reroll
        if (input == "R") {
            if (gold < rerollCost) {
                std::cout << "  Not enough gold! (Need " << rerollCost << ", have " << gold << ")\n";
                continue;
            }
            gold -= rerollCost;
            std::cout << "  Shop rerolled! (-" << rerollCost << " gold)\n";
            rerollCost += 1;
            refreshShop();
            continue;
        }

        // Modifier manager
        if (input == "M") {
            openModifierManager(gold, activeModifiers);
            continue;
        }

        // Leave shop
        if (input == "0") {
            std::cout << "  Leaving shop...\n";
            break;
        }

        // Buy item
        try {
            int choice = std::stoi(input);
            if (choice < 1 || choice > (int)currentItems.size()) {
                std::cout << "  Invalid choice!\n";
                continue;
            }
            buyItem(choice - 1, gold, activeModifiers);
        }
        catch (...) {
            std::cout << "  Invalid input.\n";
        }
    }
}
