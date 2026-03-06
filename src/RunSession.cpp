#include "RunSession.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <limits>

RunSession::RunSession()
    : round(1),
      gold(5),
      handsPerRound(4),
      discardsPerRound(4),
      gameOver(false),
      sortByRank(true),
      sortBySuit(false),
      scoreTarget(300),
      shopSystem(nullptr)
{}

RunSession::~RunSession() {
    delete shopSystem;
    for (IModifier* m : activeModifiers) {
        delete m;
    }
    activeModifiers.clear();
}

void RunSession::startRun() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "       Welcome to the Game!\n";
    std::cout << "========================================\n";
    std::cout << "  Survive as long as you can!\n";
    std::cout << "  Beat the score target each round.\n";
    std::cout << "  Buy modifiers to boost your score.\n";
    std::cout << "========================================\n\n";

    shopSystem = new ShopSystem();

    while (true) {
        displayStatus();
        playRound();

        // Check if player survived
        if (gameOver) {
            break;
        }

        // Scale target for next round
        scoreTarget = (int)(scoreTarget * 1.5);
        round++;

        // Open shop between rounds
        openShop();
    }

    std::cout << "\n========================================\n";
    std::cout << "              G A M E  O V E R\n";
    std::cout << "========================================\n";
    std::cout << "  You survived " << (round == 0 ? round : round - 1) << " round(s)!\n";
    std::cout << "  Final Gold: " << gold << "\n";
    std::cout << "========================================\n";
}

void RunSession::playRound() {
    std::cout << "\n=== ROUND " << round << " ===\n";
    std::cout << "  Target Score: " << scoreTarget << "\n";

    deck.shuffle();

    int roundScore = 0;
    int handsLeft = handsPerRound;
    int discardsLeft = discardsPerRound;

    // Draw initial hand
    std::vector<Card> hand = deck.drawMultiple(8);

    while (handsLeft > 0 && roundScore < scoreTarget) {
        std::cout << "----------------------------------------\n";
        std::cout << "  Hands left: " << handsLeft << " | Discards left: " << discardsLeft << "\n";
        std::cout << "  Round Score: " << roundScore << " / " << scoreTarget << "\n";
        std::cout << "----------------------------------------\n\n";

        sortHand(hand);
        displayHand(hand);

        PlayerAction action = getPlayerAction(hand, handsLeft, discardsLeft);

        // Sort
        if (!action.sortType.empty()) {
            if (action.sortType == "RANK") {
                sortByRank = true;
                sortBySuit = false;
                std::cout << "  Hand sorted by rank.\n\n";
            } else {
                sortBySuit = true;
                sortByRank = false;
                std::cout << "  Hand sorted by suit.\n\n";
            }
            sortHand(hand);
            continue;
        }

        // Quit
        if (action.isQuit) {
            std::cout << "\n  Quitting run...\n";
            round = 0;
            return;
        }

        if (action.selectedCards.empty()) {
            std::cout << "  No valid cards selected, try again.\n\n";
            continue;
        }

        // Discard
        if (action.isDiscard) {
            if (discardsLeft <= 0) {
                std::cout << "  No discards left!\n\n";
                continue;
            }
            discardsLeft--;
            std::cout << "  Discarded " << action.selectedCards.size() << " card(s). Drawing replacements...\n\n";

            // Remove discarded cards from hand
            for (const Card& dis : action.selectedCards) {
                for (int i = 0; i < (int)hand.size(); i++) {
                    if (hand[i].toString() == dis.toString()) {
                        hand.erase(hand.begin() + i);
                        break;
                    }
                }
            }

            // Draw back up to 8
            int toDraw = 8 - (int)hand.size();
            if (deck.remaining() < toDraw) { deck.reset(); deck.shuffle(); }
            std::vector<Card> newCards = deck.drawMultiple(toDraw);
            hand.insert(hand.end(), newCards.begin(), newCards.end());
            continue;
        }

        // Calculate score
        std::cout << "\n-- Score Calculation --\n";
        ScoreContext ctx = scoringSystem.calculate(action.selectedCards, activeModifiers);
        roundScore += ctx.finalScore();
        handsLeft--;

        std::cout << "\n  Hand Score : " << ctx.finalScore() << "\n";
        std::cout << "  Round Score: " << roundScore << " / " << scoreTarget << "\n";

        // Remove played cards and draw back up to 8
        if (handsLeft > 0 && roundScore < scoreTarget) {
            // Remove selected cards from hand
            for (const Card& sel : action.selectedCards) {
                for (int i = 0; i < (int)hand.size(); i++) {
                    if (hand[i].toString() == sel.toString()) {
                        hand.erase(hand.begin() + i);
                        break;
                    }
                }
            }
            // Draw back up to 8
            int toDraw = 8 - (int)hand.size();
            if (deck.remaining() < toDraw) deck.reset();
            std::vector<Card> newCards = deck.drawMultiple(toDraw);
            hand.insert(hand.end(), newCards.begin(), newCards.end());

            std::cout << "\n  Drawing " << toDraw << " new card(s)...\n\n";
        }
    }

    // Round result
    if (roundScore >= scoreTarget) {
        int leftover  = roundScore - scoreTarget;
        int goldEarned = 5 + (leftover / 50);
        gold += goldEarned;

        std::cout << "\n========================================\n";
        std::cout << "  ROUND CLEAR!\n";
        std::cout << "  Final Score : " << roundScore << "\n";
        std::cout << "  Target      : " << scoreTarget << "\n";
        std::cout << "  Leftover    : " << leftover << "\n";
        std::cout << "  Gold Earned : +" << goldEarned << " (5 base + " << leftover/100 << " bonus)\n";
        std::cout << "  Total Gold  : " << gold << "\n";
        std::cout << "========================================\n";
    }
    else {
        std::cout << "\n========================================\n";
        std::cout << "  You reached Round " << round << " before falling!\n";
        std::cout << "  Final Score : " << roundScore << "\n";
        std::cout << "  Target      : " << scoreTarget << "\n";
        std::cout << "  You needed  : " << (scoreTarget - roundScore) << " more points.\n";
        std::cout << "========================================\n";
        gameOver = true; // Signal game over
    }
}

void RunSession::openShop() {
    std::cout << "\n-- Next Round Target: " << scoreTarget << " --\n";
    shopSystem->refreshShop();
    shopSystem->openShop(gold, activeModifiers);

    // Flush leftover newline from shop's cin >> input
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void RunSession::displayStatus() const {
    std::cout << "----------------------------------------\n";
    std::cout << "  Round : " << round << "\n";
    std::cout << "  Gold  : " << gold << "\n";
    std::cout << "  Target: " << scoreTarget << "\n";
    displayModifiers();
    std::cout << "----------------------------------------\n";
}

void RunSession::displayModifiers() const {
    if (activeModifiers.empty()) {
        std::cout << "  Modifiers: None\n";
        return;
    }
    std::cout << "  Modifiers:\n";
    for (IModifier* m : activeModifiers) {
        std::cout << "    - " << m->getName() << "\n";
    }
}

RunSession::PlayerAction RunSession::getPlayerAction(const std::vector<Card>& hand, int handsLeft, int discardsLeft) {
    PlayerAction action;
    action.isDiscard = false;
    action.isQuit = false;
    action.sortType = "";

    std::cout << "\n  Select cards then action:\n";
    std::cout << "    P  = play  | D  = discard | Q = quit\n";
    std::cout << "    RS = sort by rank         | SS = sort by suit\n";
    std::cout << "    H  = hand reference\n";
    std::cout << "    e.g. '1 3 5 P' or '2 4 D'\n";
    std::cout << "  > ";

    std::string line;
    std::getline(std::cin, line);

    // Convert to uppercase
    for (char& c : line) {
        c = toupper(c);
    }

    // Check for quit
    if (line.find('Q') != std::string::npos) {
        std::cout << "  Are you sure you want to quit? (Y/N): ";
        std::string confirm;
        std::getline(std::cin, confirm);
        for (char& c : confirm) c = toupper(c);
        if (confirm.find('Y') != std::string::npos) {
            action.isQuit = true;
        }
        return action;
    }

    // Check for hand reference
    if (line == "H") {
        displayHandReference();
        action.isQuit = false;
        action.isDiscard = false;
        action.selectedCards.clear();
        return action;
    }

    // Check for sort commands
    if (line == "RS") {
        action.sortType = "RANK";
        return action;
    }
    if (line == "SS") {
        action.sortType = "SUIT";
        return action;
    }

    // Check for discard
    if (line.find('D') != std::string::npos) {
        action.isDiscard = true;
    }

    // Parse card indices
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        if (token == "P" || token == "D" || token == "Q") {
            continue;
        }
        try {
            int idx = std::stoi(token);
            if (idx >= 1 && idx <= (int)hand.size()) {
                bool already = false;
                for (const Card& c : action.selectedCards) {
                    if (c.toString() == hand[idx-1].toString()) {
                        already = true;
                    }
                }
                if (!already && (int)action.selectedCards.size() < 5) {
                    action.selectedCards.push_back(hand[idx-1]);
                }
            }
        }
        catch (...) {
            continue;
        }
    }

    return action;
}

void RunSession::displayHand(const std::vector<Card>& hand) const {
    std::cout << "  Your Hand:\n";
    for (int i = 0; i < (int)hand.size(); i++) {
        std::cout << "    " << (i+1) << ". " << hand[i].toString() << " (Chips: " << hand[i].getChipValue() << ")\n";
    }
}

void RunSession::sortHand(std::vector<Card>& hand) const {
    if (sortByRank) {
        std::sort(hand.begin(), hand.end(), [](const Card& a, const Card& b) {
            return static_cast<int>(a.getRank()) > static_cast<int>(b.getRank());
        });
    }
    else if (sortBySuit) {
        std::sort(hand.begin(), hand.end(), [](const Card& a, const Card& b) {
            if (a.getSuit() != b.getSuit()) {
                return static_cast<int>(a.getSuit()) < static_cast<int>(b.getSuit());
            }
            return static_cast<int>(a.getRank()) > static_cast<int>(b.getRank());
        });
    }
}

void RunSession::displayHandReference() const {
    std::cout << "\n========================================\n";
    std::cout << "         P O K E R  H A N D S\n";
    std::cout << "========================================\n";
    std::cout << "  Hand            Chips  Mult\n";
    std::cout << "  --------------  -----  ----\n";
    std::cout << "  High Card          5     1\n";
    std::cout << "  Pair              10     2\n";
    std::cout << "  Two Pair          20     2\n";
    std::cout << "  Three of a Kind   30     3\n";
    std::cout << "  Straight          30     4\n";
    std::cout << "  Flush             35     4\n";
    std::cout << "  Full House        40     4\n";
    std::cout << "  Four of a Kind    60     7\n";
    std::cout << "  Straight Flush   100     8\n";
    std::cout << "========================================\n\n";
}
