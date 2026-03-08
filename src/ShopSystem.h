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
    static const int MODIFIER_LIMIT = 5;

    ShopSystem();

    void openShop(int& gold, std::vector<IModifier*>& activeModifiers);
    void refreshShop();

private:
    std::vector<ShopItem> allItems;
    std::vector<ShopItem> currentItems;

    void buildItemPool();
    void displayShop(int gold, int rerollCost, const std::vector<IModifier*>& activeModifiers) const;
    bool buyItem(int index, int& gold, std::vector<IModifier*>& activeModifiers);
    void openModifierManager(int& gold, std::vector<IModifier*>& activeModifiers);
    void displayModifierManager( const std::vector<IModifier*>& activeModifiers) const;
};

#endif
