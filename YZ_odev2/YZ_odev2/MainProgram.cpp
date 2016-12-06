#include "stdafx.h"
#include "MainProgram.h"
#include <iostream>
#include <conio.h>

MainProgram::MainProgram()
{
}

//Initializes key parameters with inputs from the user.
void MainProgram::init()
{
	std::cout << "Enter 6 numbers: \n";
	for (int i = 0;i < 6;i++) {
		std::cout << "Enter the " << (i+1) << ". number: ";
		std::cin >> numbers[i];
	}	
	for (int i = 0;i < 6;i++) {
		std::cout << numbers[i] << " ";
	}	
	std::cout << "\nEnter the target number: ";
	std::cin >> targetNumber;
}

void MainProgram::run()
{
	char c;
	do 
	{ // A loop, to be able to repeat the process without having to exit the program.
		system("cls");
		init();
		do 
		{ // A loop, to be able to repeat the process with the same paremeters.
			geneticAlgorithm.generatePopulation(numbers, 6, 5);
			geneticAlgorithm.evolve(targetNumber);

			std::cout << "Press R to repeat the process." << std::endl;
			std::cout << "Press E to repeat the process with the same paremeters.";
			c = _getch();
		} while (c == 'e');
	} while (c == 'r');
}



