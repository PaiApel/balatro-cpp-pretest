#include "Deck.h"
#include <algorithm>
#include <random>
#include <stdexcept>

Deck::Deck() : topIndex(0) {
    buildDeck();
}

void Deck::buildDeck() {
    cards.clear();
    for (Suit suit : {Suit::HEART, Suit::DIAMOND, Suit::SPADE, Suit::CLUB}) {
        for (int r = static_cast<int>(Rank::TWO);
                 r <= static_cast<int>(Rank::ACE); r++) {
            cards.push_back(Card(static_cast<Rank>(r), suit));
        }
    }
    topIndex = 0;
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin() + topIndex, cards.end(), g);
}

Card Deck::draw() {
    if (topIndex >= (int)cards.size()) {
        reset();
        shuffle();
    }
    return cards[topIndex++];
}

std::vector<Card> Deck::drawMultiple(int count) {
    std::vector<Card> hand;
    for (int i = 0; i < count; i++) {
        hand.push_back(draw());
    }
    return hand;
}

void Deck::reset() {
    topIndex = 0;
}

int Deck::remaining() const {
    return (int)cards.size() - topIndex;
}
