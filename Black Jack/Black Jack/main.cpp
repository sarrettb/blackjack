#include <iostream>
#include "blackjack.h"

int main()
{
	std::cout << "----- BLACK JACK -----\n\n"; 
	blackjack::Deck card_deck;
	card_deck.shuffle();
	std::cout << card_deck; 

	return EXIT_SUCCESS; 
}