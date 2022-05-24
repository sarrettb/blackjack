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

	class Card
	{
	public:
		Card(const Suite&& s, const Face_Value&& f);
		Card(const Suite& s, const Face_Value& f);
		Card();
		Suite suite;
		Face_Value value;
		static const int NUM_SUITES = 4;
		static const int NUM_FACE_VALUE = 13;
		static constexpr const char* SUITE_NAMES[NUM_SUITES] = { "Hearts", "Diamonds", "Spades", "Clubs" };
		static constexpr const char* FACE_VALUE_NAMES[NUM_FACE_VALUE] = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };
		friend std::ostream& operator << (std::ostream& out, const Card& c);
	};

	std::ostream& operator << (std::ostream& out, const Card& c);

	class Deck
	{
	public: 
		Deck(bool loadDeck = true);
		Card& operator [] (size_t index);
		static const int NUM_CARDS = 52; 

	private:
		Card deck[NUM_CARDS];
		void load_deck(); 
	};

	class Dealer
	{
	public:
		Dealer(int nDecks = 1);
		~Dealer(); 
		void shuffle(int numShuffles = 5); 
		Card deal_card();
		friend std::ostream& operator << (std::ostream& out, const Dealer& d);
		const int num_decks() const { return numDecks; }
		const int totalCards() const { return Deck::NUM_CARDS * numDecks; }
		const Card& operator () (size_t deckIndex, size_t cardIndex) const ; 

	private:
		int numDecks;
		int cardIndex; 
		int deckIndex;
		Deck* card_deck;
		void swap_cards(Card& a, Card& b);
	};

	std::ostream& operator << (std::ostream& out, const Dealer& d);

	/* A Hand is a linked list of Cards
	* I could have used std::list or std::vector 
	* but I want some practice coding some data structures 
	*/
	class CardNode
	{
	public:
		CardNode();
		~CardNode();
		void add(const Card& c);

		Card card; 
		CardNode* nextCard;

	private:
		// internal functionality
		CardNode(const Card& c);

	};

	class Hand
	{
	public:
		Hand();  
		~Hand(); 
		void show_hand(); 
		void init_hand(const Card& c1, const Card& c2);
		void draw_card(const Card& c); 
		
	private:
		CardNode* listOfCards;
	};

	class Player
	{
	public:

		Player(); 
		void set_hand(Card& c1, Card& c2);
		void show_hand() { hand.show_hand(); }

	private:
		Hand hand; 
	};

	namespace tester
	{
		template <typename T> 
		struct Test_Result
		{
			bool bSuccess;
			T data;
		};

		Test_Result<Card> test_shuffle(const Dealer& d);
	}

	void play(); 
	void init_game(); 

}

#endif 