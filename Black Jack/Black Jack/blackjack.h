#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <iostream>

namespace blackjack
{
	enum class Suite
	{
		HEARTS = 0,
		DIAMONDS = 1,
		SPADES = 2,
		CLUBS = 3
	};

	enum class Face_Value
	{
		ACE = 0,
		TWO = 1,
		THREE = 2,
		FOUR = 3,
		FIVE = 4,
		SIX = 5,
		SEVEN = 6,
		EIGHT = 7,
		NINE = 8,
		TEN = 9,
		JACK = 10,
		QUEEN = 11,
		KING = 12
	};

	struct Card
	{
		Suite suite;
		Face_Value value;
		Card(const Suite&& s, const Face_Value&& f) : suite(s), value(f) {}; 
		Card(const Suite& s, const Face_Value& f) : suite(s), value(f) {}; 
		Card() : suite(Suite::HEARTS), value(Face_Value::ACE) {}; // default values 
	};

	class Deck
	{
	public: 
		Deck(); 
		const Card& operator [] (size_t index) const;
		friend std::ostream& operator << (std::ostream& out, const Deck& d);
		void shuffle(int numShuffles = 5);

	private:
		static const int NUM_CARDS = 52;
		static const int NUM_SUITES = 4; 
		static const int NUM_FACE_VALUE = 13;
		static constexpr const char* SUITE_NAMES[NUM_SUITES] = {"Hearts", "Diamonds", "Spades", "Clubs" };
		static constexpr const char* FACE_VALUE_NAMES[NUM_FACE_VALUE] = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };
		Card deck[NUM_CARDS];
	};

	std::ostream& operator << (std::ostream& out, const Deck& d);

}

#endif 