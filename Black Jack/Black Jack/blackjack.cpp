#include "blackjack.h"

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
	for (int i = 0; i < NUM_SUITES; i++)
	{
		for (int j = 0; j < NUM_FACE_VALUE; j++)
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
blackjack::Dealer::Dealer(int numDecks) : num_decks(numDecks)
{
	card_deck = new Deck[num_decks];
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
			for (int k = 0; k < num_decks; k++)
			{
				randomCardIndex = j + (rand() % (Deck::NUM_CARDS - j));
				randomDeckIndex = k + (rand() % (num_decks - k));
				swap_cards(card_deck[randomDeckIndex][randomCardIndex], card_deck[k][j]);
			}
		}
	}
}

void blackjack::Dealer::swap_cards(Card& a, Card& b)
{
	static Card temp; 
	temp = b; 
	b = a;
	a = temp;
}

/* Prints the deck to console */  
std::ostream& blackjack::operator << (std::ostream& out, const blackjack::Dealer& d)
{
	Card card;
	for (int i = 0; i < d.num_decks; i++)
	{
		for (int j = 0; j < Deck::NUM_CARDS; j++)
		{
			card = d.card_deck[i][j];
			out << Deck::FACE_VALUE_NAMES[(int)card.value] << " of " << Deck::SUITE_NAMES[(int)card.suite] << std::endl;
		}
	}
	return out;
}

const blackjack::Card& blackjack::Dealer::operator () (size_t deckIndex, size_t cardIndex) const
{
	if (deckIndex >= num_decks)
	{
		throw std::runtime_error("ERROR: Invalid Deck Index using Dealer () operator\n");
	}

	else if (cardIndex >= Deck::NUM_CARDS)
	{
		throw std::runtime_error("ERROR: Invalid Card Index using Dealer() operator\n");
	}

	return card_deck[deckIndex][cardIndex]; 
}

blackjack::tester::Test_Result<blackjack::Card> blackjack::tester::test_shuffle(const Dealer& d)
{

	Card card_search;
	Card temp;
	Deck tempDeck; // unshuffled deck
	int count = 0;
	int count_goal = d.numDecks();
	for (int i = 0; i < Deck::NUM_CARDS; i++)
	{
		card_search = tempDeck[i]; 
		for (int j = 0; j < d.numDecks(); j++)
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
	const int NUM_DECKS = 8; 
	Dealer dealer(NUM_DECKS);
	dealer.shuffle(); 
	std::cout << "----- SHUFFLED DECK -----\n\n" << dealer << std::endl;
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
	

}
