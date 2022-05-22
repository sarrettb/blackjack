#include <iostream>
#include "blackjack.h"

int main()
{
	std::cout << "----- BLACK JACK CONSOLE APPLICATION -----\n\n"; 

	try
	{
		blackjack::play();
	}

	catch (std::runtime_error& error)
	{
		std::cout << error.what(); 
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS; 
}