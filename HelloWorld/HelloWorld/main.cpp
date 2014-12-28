#include <iostream>
#include <string>
#include <random>

#define START 0
#define END 9
#define RUN_TOTAL 100000

int main(int argc, char** argv)
{
	/* Declare variables. */
	int total = 0, temp = 0, RANDOM_VARIABLES = 1, size = 1, *counts = NULL;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(START, END);

	/* Prompt user input. */
	std::cout << "Enter number of variables: ";
	std::cin >> RANDOM_VARIABLES;
	std::cout << std::endl;

	/* Allocate space for the array. */
	size = (END - START) * RANDOM_VARIABLES + 1;
	counts = (int*) malloc(size * sizeof(int));
	for (int i = 0; i < size; i++)
		counts[i] = 0;

	/* Generate numbers. */
	for (int i = 0; i < RUN_TOTAL; i++)
	{
		temp = 0;
		for (int j = 0; j < RANDOM_VARIABLES; j++) 
			temp += distribution(generator);
		counts[temp]++;
		total++;
	}

	/* Display output. */
	for (int i = 0; i < size; i++)
		std::cout << "Count of " << i << ": " << counts[i] << " - " << double(counts[i] * 100.0) / total << "%" << std::endl;

	free(counts);
}
