#pragma once
#include "GeneticAlgorithm.h"
#include "OperationTree.h"

//The class that runs and uses other classes when necessary.
class MainProgram
{
private:
	int numbers[6];//The array that stores the numbers.
	int amountOfNumbers;
	int targetNumber;
	GeneticAlgorithm geneticAlgorithm;
public:
	MainProgram();
	void init();//Initializes the necessary variables by getting input from the user.
	void run();//Runs the genetic algorithm.
};
