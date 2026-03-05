#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>

class Deck {
public:
    Deck();

    void shuffle();
    Card draw();
    std::vector<Card> drawMultiple(int count);
    void reset();
    int remaining() const;

private:
    std::vector<Card> cards;
    int topIndex;

    void buildDeck();
};

#endif
