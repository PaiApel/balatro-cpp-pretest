#ifndef RUNSESSION_H
#define RUNSESSION_H

#include "Deck.h"
#include "ScoringSystem.h"
#include "ShopSystem.h"
#include "modifiers/IModifier.h"
#include <vector>

class RunSession {
public:
    RunSession();
    ~RunSession();

    void startRun();

private:
    // Game state
    int round;
    int gold;
    int handsPerRound;
    int discardsPerRound;
    int scoreTarget;
    bool gameOver;

    // Round reward
    int baseGold;
    int leftoverDivisor;

    // Hand display state
    bool sortByRank;
    bool sortBySuit;

    // Core systems
    Deck deck;
    ScoringSystem scoringSystem;
    ShopSystem* shopSystem;

    // Active modifiers owned by player
    std::vector<IModifier*> activeModifiers;

    // Private helpers
    void playRound();
    void openShop();
    void displayStatus() const;
    void displayHand(const std::vector<Card>& hand) const;
    void displayActiveModifiers() const;
    void sortHand(std::vector<Card>& hand) const;
    void displayHandReference() const;
    void updateGoldFormula();

    struct PlayerAction {
        std::vector<Card> selectedCards;
        bool isDiscard;
        bool isQuit;
        bool isHelp;
        std::string sortType;
    };

    PlayerAction getPlayerAction(const std::vector<Card>& hand, int handsLeft, int discardsLeft);
};

#endif
