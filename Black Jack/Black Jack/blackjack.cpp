#include "blackjack.h"

/* Constructor of Deck, loads deck unshuffled */
blackjack::Deck::Deck()
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
const blackjack::Card& blackjack::Deck::operator [] (size_t index) const
{
	if (index >= Deck::NUM_CARDS)
	{
		throw std::runtime_error("ERROR: Invalid indexing of deck\n");
	}

	return deck[index];
}

/* Prints the deck to console */
std::ostream& blackjack::operator << (std::ostream& out, const blackjack::Deck& d)
{
	blackjack::Card temp;
	for (int i = 0; i < blackjack::Deck::NUM_CARDS; i++)
	{
		temp = d[i];
		out << Deck::FACE_VALUE_NAMES[(int)temp.value] << " of " << Deck::SUITE_NAMES[(int)temp.suite] << std::endl;
	}
	return out;
}

/* One shuffle is completed by swapping a random index with the ith index and iterating over the whole card deck
*  Perform "N" shuffles by setting numShuffles to a desired value, default is 5 
*/
void blackjack::Deck::shuffle(int numShuffles)
{
	srand(time(NULL));
	for (int i = 0; i < numShuffles; i++)
	{
		for (int j = 0; j < NUM_CARDS; j++)
		{
			int randomIndex = j + (rand() % (52 - j));
			std::swap(deck[j], deck[randomIndex]);
		}
	}
}
