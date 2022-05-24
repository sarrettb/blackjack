#include "blackjack.h"
#include <string>

/* Various Card Constructors */
blackjack::Card::Card(const Suite&& s, const Face_Value&& f) : suite(s), value(f) {};
blackjack::Card::Card(const Suite& s, const Face_Value& f) : suite(s), value(f) {};
blackjack::Card::Card() : suite(Suite::HEARTS), value(Face_Value::ACE) {}; // default values 

/* To use std::cout << card */
std::ostream& blackjack::operator << (std::ostream& out, const Card& c)
{
	out << Card::FACE_VALUE_NAMES[(int)c.value] << " of " << Card::SUITE_NAMES[(int)c.suite];
	return out;
}

/* Constructor of Deck, loads deck unshuffled */
blackjack::Deck::Deck(bool loadDeck)
{
	if (loadDeck)
		load_deck();
}

/* Loads unshuffled deck */
void blackjack::Deck::load_deck()
{
	int index = 0;
	for (int i = 0; i < Card::NUM_SUITES; i++)
	{
		for (int j = 0; j < Card::NUM_FACE_VALUE; j++)
		{
			deck[index++] = Card((Suite)i, (Face_Value)j);
		}
	}
}

/* Retrieve the card at specified index, throws std::runtime_error if invalid index */
blackjack::Card& blackjack::Deck::operator [] (size_t index)
{
	if (index >= Deck::NUM_CARDS)
	{
		throw std::runtime_error("ERROR: Invalid Card Index using Deck [] Operator\n");
	}

	return deck[index];
}

/* Constructor, allocate memory for the decks */
blackjack::Dealer::Dealer(int nDecks) : numDecks(nDecks), cardIndex(0), deckIndex(0)
{
	card_deck = new Deck[numDecks];
}

/* Destructor, Free Dealer Resources */
blackjack::Dealer::~Dealer()
{
	delete[] card_deck;
}

/* Shuffles the whole deck
* Swaps a random card, from a  random deck, with the kth deck and jth card
* Performs this operation "numShuffles" times, by default it is 5
*/
void blackjack::Dealer::shuffle(int numShuffles)
{
	srand(time(NULL));
	int r;
	int randomCardIndex;
	int randomDeckIndex;
	for (int i = 0; i < numShuffles; i++)
	{
		for (int j = 0; j < Deck::NUM_CARDS; j++)
		{
			for (int k = 0; k < numDecks; k++)
			{
				randomCardIndex = j + (rand() % (Deck::NUM_CARDS - j));
				randomDeckIndex = k + (rand() % (numDecks - k));
				swap_cards(card_deck[randomDeckIndex][randomCardIndex], card_deck[k][j]);
			}
		}
	}
}

/* Swap the two cards for the shuffle algorithm */
void blackjack::Dealer::swap_cards(Card& a, Card& b)
{
	static Card temp; 
	temp = b; 
	b = a;
	a = temp;
}

/* Deals one card */
blackjack::Card blackjack::Dealer::deal_card()
{
	if (cardIndex >= Deck::NUM_CARDS)
	{
		cardIndex = 0;
		deckIndex++; 
	}

	if (deckIndex >= numDecks)
		throw std::runtime_error("ERROR: Ran out of cards\n");

	return card_deck[cardIndex++][deckIndex];
}

/* Prints the deck to console */  
std::ostream& blackjack::operator << (std::ostream& out, const blackjack::Dealer& d)
{
	for (int i = 0; i < d.numDecks; i++)
	{
		for (int j = 0; j < Deck::NUM_CARDS; j++)
		{
			out << d.card_deck[i][j] << std::endl;
		}
	}
	return out;
}

/* Retrieve the card with a deck and card index, throw error if an invalid index exists */
const blackjack::Card& blackjack::Dealer::operator () (size_t deckIndex, size_t cardIndex) const
{
	if (deckIndex >= numDecks)
	{
		throw std::runtime_error("ERROR: Invalid Deck Index using Dealer () operator\n");
	}

	else if (cardIndex >= Deck::NUM_CARDS)
	{
		throw std::runtime_error("ERROR: Invalid Card Index using Dealer() operator\n");
	}

	return card_deck[deckIndex][cardIndex]; 
}

blackjack::CardNode::CardNode(const Card& c)
{
	nextCard = nullptr;
	card = c; 
}

blackjack::CardNode::CardNode()
{
	nextCard = nullptr;
}

/* How to delete nodes? */
blackjack::CardNode::~CardNode()
{
	
}

void blackjack::CardNode::add(const Card& c)
{
	nextCard = new CardNode(c);
}

blackjack::Hand::Hand()
{
	listOfCards = nullptr;
}

/* How to delete list */
blackjack::Hand::~Hand()
{
	if (listOfCards != nullptr)
		delete listOfCards;
}

void blackjack::Hand::draw_card(const Card& c)
{
	if (listOfCards == nullptr)
		throw std::runtime_error("ERROR: Must call init_hand() first\n");
	listOfCards->add(c);
}

void blackjack::Hand::init_hand(const Card& c1, const Card& c2)
{
	listOfCards = new CardNode; 
	listOfCards->add(c1);
	listOfCards->add(c2); 
}

void blackjack::Hand::show_hand()
{
	std::cout << "Your Hand is: \n";
	CardNode* temp = listOfCards;
	do
	{
		std::cout << temp->card << std::endl;
		temp = temp->nextCard;
	} while (temp != nullptr);
}

blackjack::Player::Player() {};
void blackjack::Player::set_hand(Card& c1, Card& c2)
{
	hand.init_hand(c1, c2); 
}

/* Test to see if every card is still in the deck and did not get lost somehow */
blackjack::tester::Test_Result<blackjack::Card> blackjack::tester::test_shuffle(const Dealer& d)
{

	Card card_search;
	Card temp;
	Deck tempDeck; // unshuffled deck
	int count = 0;
	int count_goal = d.num_decks();
	for (int i = 0; i < Deck::NUM_CARDS; i++)
	{
		card_search = tempDeck[i]; 
		for (int j = 0; j < d.num_decks(); j++)
		{
			for (int k = 0; k < Deck::NUM_CARDS; k++)
			{
				temp = d(j, k);
				if (card_search.suite == temp.suite && card_search.value == temp.value)
					count++; 
			}
		}
		if (count != count_goal)
		{
			std::cout << "Count Number: " << count;
			return { false, card_search };
		}
		count = 0;
	}
	return { true, card_search }; 
}

/* Application */
void blackjack::play()
{
	const int NUM_DECKS = 8; // Vegas usually uses 6-8 decks 
	Dealer dealer(NUM_DECKS);
	dealer.shuffle(); 


	std::cout << "----- SHUFFLED DECK -----\n\n" << dealer << std::endl;

	/*
	// Tester Code 
	std::cout << "----- SHUFFLE TEST ------\n\n";
	auto result = tester::test_shuffle(dealer);
	if (result.bSuccess)
	{
		std::cout << "The Dealer properly shuffled the deck\n";
	}

	else
	{
		std::cout << std::string("The Dealer improperly shuffled the deck, mistake on ")
					+ std::string(Deck::FACE_VALUE_NAMES[((int)result.data.value)]) + " of "
					+ std::string(Deck::SUITE_NAMES[((int)result.data.suite)]) 
			<< std::endl;
	}
	*/ 

	Player player; 
	Card firstTwo[2]; 

	for (int i = 0; i < 2; i++)
		firstTwo[i] = dealer.deal_card(); 

	player.set_hand(firstTwo[0], firstTwo[1]); 
	player.show_hand(); 

	

}
