#include "Card.h"

Card::Card(Rank rank, Suit suit) : rank(rank), suit(suit) {}

Rank Card::getRank() const {
    return rank;
}

Suit Card::getSuit() const {
    return suit;
}

int Card::getChipValue() const {
    int r = static_cast<int>(rank);
    if (r >= 2 && r <= 10) return r;
    if (r == static_cast<int>(Rank::JACK))  return 10;
    if (r == static_cast<int>(Rank::QUEEN)) return 10;
    if (r == static_cast<int>(Rank::KING))  return 10;
    if (r == static_cast<int>(Rank::ACE))   return 11;
    return 0;
}

std::string Card::toString() const {
    std::string rankStr;
    switch (rank) {
        case Rank::TWO: rankStr = "2";  break;
        case Rank::THREE: rankStr = "3";  break;
        case Rank::FOUR: rankStr = "4";  break;
        case Rank::FIVE: rankStr = "5";  break;
        case Rank::SIX: rankStr = "6";  break;
        case Rank::SEVEN: rankStr = "7";  break;
        case Rank::EIGHT: rankStr = "8";  break;
        case Rank::NINE: rankStr = "9";  break;
        case Rank::TEN: rankStr = "10"; break;
        case Rank::JACK: rankStr = "J";  break;
        case Rank::QUEEN: rankStr = "Q";  break;
        case Rank::KING: rankStr = "K";  break;
        case Rank::ACE: rankStr = "A";  break;
    }

    std::string suitStr;
    switch (suit) {
        case Suit::HEART: suitStr = "Heart"; break;
        case Suit::DIAMOND: suitStr = "Diamond";  break;
        case Suit::SPADE: suitStr = "Spade"; break;
        case Suit::CLUB: suitStr = "Club";  break;
    }

    return rankStr + " of " + suitStr;
}
