#include "blackjack.h"
#include <thread> 

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

	return card_deck[deckIndex][cardIndex++];
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

/* Build an empty hand */
blackjack::Hand::Hand()
{
	head = nullptr;
}

/* Destroy the hand */
blackjack::Hand::~Hand()
{
	CardNode* tempA = head;
	CardNode* tempB = nullptr; 
	while (tempA != nullptr)
	{
		tempB = tempA;
		tempA = tempA->next;
		delete tempB;
	}
}

/* Add a card to the hand */
void blackjack::Hand::add(const Card& c)
{
	CardNode** tempHead = &head;
	CardNode* temp = nullptr;

	/* Node to insert at end of list */ 
	CardNode* lastNode = new CardNode;
	lastNode->next = nullptr;
	lastNode->card = c;

	if (*tempHead == nullptr)
		*tempHead = lastNode;

	else
	{
		temp = *tempHead;
		/* Traverse list to get last node*/
		while (temp->next != nullptr)
			temp = temp->next;

		temp->next = lastNode;
	}
}

/* Print the hand to the console */
std::ostream& blackjack::operator << (std::ostream& out, Hand& d)
{
	Hand::CardNode* node = d.head;
	while (node != nullptr)
	{
		out << node->card;
		node = node->next;
		if (node != nullptr)
			out << ", ";
	}
	out << "\n\n";
	return out;
}

/* Add a card to the Player's hand */
void blackjack::Player::draw_card(const Card& c)
{
	hand.add(c); 
}

void blackjack::Player::set_name(const char* name)
{
	playerName = name;
}

/* Print Player's hand to console */
void blackjack::Player::show_hand()
{
	std::cout << playerName << "'s Cards: " << hand;
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
	Player player;
	std::string name;

	std::cout << "Enter Player's Name: ";
	std::cin >> name;
	std::cout << std::endl;
	player.set_name(name.c_str());

	using namespace std::chrono_literals;
	std::cout << "Dealer Shuffling...\n\n";
	dealer.shuffle(); 
	std::this_thread::sleep_for(2s); // simulate shuffling
	
	std::cout << "Dealing Cards...\n\n"; 
	std::this_thread::sleep_for(1s);
	for (int i = 0; i < 2; i++)
	{
		player.draw_card(dealer.deal_card());
	}

	player.show_hand(); 
	

}
