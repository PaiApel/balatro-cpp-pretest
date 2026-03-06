#ifndef SHOPSYSTEM_H
#define SHOPSYSTEM_H

#include "ModifierFactory.h"
#include "modifiers/IModifier.h"
#include <vector>
#include <string>

struct ShopItem {
    std::string modifierType;
    std::string displayName;
    std::string description;
    int cost;
    bool sold;
};

class ShopSystem {
public:
    static const int SHOP_SIZE = 3;

    ShopSystem();

    void openShop(int& gold, std::vector<IModifier*>& activeModifiers);
    void refreshShop();

private:
    std::vector<ShopItem> allItems;
    std::vector<ShopItem> currentItems;

    void buildItemPool();
    void displayShop(int gold) const;
    bool buyItem(int index, int& gold, std::vector<IModifier*>& activeModifiers);
};

#endif
