#ifndef CARD_H
#define CARD_H

#include <string>

enum class Suit {
    HEART,
    DIAMOND,
    SPADE,
    CLUB
};

enum class Rank {
    TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT,
    NINE, TEN, JACK, QUEEN, KING, ACE
};

class Card {
public:
    Card(Rank rank, Suit suit);

    Rank getRank() const;
    Suit getSuit() const;
    int getChipValue() const;
    std::string toString() const;

private:
    Rank rank;
    Suit suit;
};

#endif
