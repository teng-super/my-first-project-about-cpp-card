#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <stdexcept>

// Card suits
enum class Suit { Spades, Hearts, Diamonds, Clubs };

// Card ranks
enum class Rank {
    Two = 2, Three, Four, Five, Six, Seven, Eight,
    Nine, Ten, Jack, Queen, King, Ace
};

// A single playing card
struct Card {
    Suit suit;
    Rank rank;

    std::string toString() const {
        static const std::map<Rank, std::string> rankNames = {
            {Rank::Two, "2"}, {Rank::Three, "3"}, {Rank::Four, "4"},
            {Rank::Five, "5"}, {Rank::Six, "6"}, {Rank::Seven, "7"},
            {Rank::Eight, "8"}, {Rank::Nine, "9"}, {Rank::Ten, "10"},
            {Rank::Jack, "J"}, {Rank::Queen, "Q"}, {Rank::King, "K"},
            {Rank::Ace, "A"}
        };
        static const std::map<Suit, std::string> suitNames = {
            {Suit::Spades, "♠"}, {Suit::Hearts, "♥"},
            {Suit::Diamonds, "♦"}, {Suit::Clubs, "♣"}
        };
        return rankNames.at(rank) + suitNames.at(suit);
    }
};

// A deck of 52 cards
class Deck {
public:
    Deck() {
        for (Suit s : {Suit::Spades, Suit::Hearts, Suit::Diamonds, Suit::Clubs}) {
            for (int r = 2; r <= 14; ++r) {
                cards.push_back({s, static_cast<Rank>(r)});
            }
        }
    }

    void shuffle() {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::shuffle(cards.begin(), cards.end(), rng);
    }

    Card deal() {
        if (cards.empty()) {
            throw std::runtime_error("Cannot deal from an empty deck");
        }
        Card top = cards.back();
        cards.pop_back();
        return top;
    }

    int remaining() const {
        return static_cast<int>(cards.size());
    }

private:
    std::vector<Card> cards;
};

// A player's hand
class Hand {
public:
    void addCard(const Card& card) {
        cards.push_back(card);
    }

    void show() const {
        for (const auto& c : cards) {
            std::cout << c.toString() << " ";
        }
        std::cout << std::endl;
    }

    int size() const {
        return static_cast<int>(cards.size());
    }

private:
    std::vector<Card> cards;
};

int main() {
    std::cout << "=== Simple Card Game Framework ===" << std::endl;

    Deck deck;
    deck.shuffle();
    std::cout << "Deck shuffled. Cards remaining: " << deck.remaining() << std::endl;

    const int numPlayers = 2;
    const int cardsPerPlayer = 5;
    std::vector<Hand> players(numPlayers);

    for (int i = 0; i < cardsPerPlayer; ++i) {
        for (int p = 0; p < numPlayers; ++p) {
            players[p].addCard(deck.deal());
        }
    }

    for (int p = 0; p < numPlayers; ++p) {
        std::cout << "Player " << (p + 1) << "'s hand: ";
        players[p].show();
    }

    std::cout << "Cards remaining in deck: " << deck.remaining() << std::endl;

    return 0;
}
